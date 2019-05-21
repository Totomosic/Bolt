import sys
import os

import src.filetypes as ft
import src.xml.XMLwriter as xml
import src.res_handlers.settings_data as settings
import src.res_handlers.resource_handlers as res
import src.error_handler as err

MODEL_SETTINGS_FILE = {
    "Name" : settings.SettingsAttribute(settings.TYPE_STRING, ["*"], "__FILENAME__")
}

BANNED_STRING_TOKENS = ["\n", "\r", "\t", " "]
DEFAULT_VERTEX = [0, 0, 0]
DEFAULT_NORMAL = [0, 1, 0]
DEFAULT_UV = [0, 0]

def string_to_float(string):
    try:
        return float(string)
    except ValueError:
        return None

def string_to_index(string, modifier = -1):
    try:
        return int(string) + modifier
    except ValueError:
        return -1

def remove_empty_strings(lst):
    result = []
    for item in lst:
        if len(item) > 0 and item not in BANNED_STRING_TOKENS:
            result.append(item)
    return result

def flatten_and_to_string(lst_of_lsts):
    result = []
    for lst in lst_of_lsts:
        for val in lst:
            result.append(str(val))
    return result

def list_to_string(lst):
    result = []
    for val in lst:
        result.append(str(val))
    return result

def process_vertex_line(line):
    parts = remove_empty_strings(line.split(" ")[1:])
    if len(parts) != 3:
        err.throw_error(err.ERROR_TYPE_INVALID_FILE, "invalid vertex dimension in obj file")
    return [string_to_float(parts[0]), string_to_float(parts[1]), string_to_float(parts[2])]

def process_normal_line(line):
    parts = remove_empty_strings(line.split(" ")[1:])
    if len(parts) != 3:
        err.throw_error(err.ERROR_TYPE_INVALID_FILE, "invalid normal dimension in obj file")
    return [string_to_float(parts[0]), string_to_float(parts[1]), string_to_float(parts[2])]

def process_uv_line(line):
    parts = remove_empty_strings(line.split(" ")[1:])
    if len(parts) < 2:
        err.throw_error(err.ERROR_TYPE_INVALID_FILE, "invalid uv dimension in obj file")
    return [string_to_float(parts[0]), string_to_float(parts[1])]

def process_face_line(line):
    parts = remove_empty_strings(line.split(" ")[1:])
    if len(parts) == 3:
        return process_3_face_line(parts)
    if len(parts) == 4:
        return process_4_face_line(parts)
    err.throw_error(err.ERROR_TYPE_INVALID_FILE, "invalid face dimension in obj file")
    return None

def process_3_face_line(parts):
    result = [[]]
    for part in parts:
        vtn = part.split("/")
        if len(vtn) == 1:
            vtn = [vtn[0], -1, -1]
        if len(vtn) != 3:
            err.throw_error(err.ERROR_TYPE_INVALID_FILE, "invalidi face specfication in obj file")
        vertexIndex = string_to_index(vtn[0], -1)
        uvIndex = string_to_index(vtn[1], -1)
        normalIndex = string_to_index(vtn[2], -1)
        result[0].append([vertexIndex, uvIndex, normalIndex])
    return result

def process_4_face_line(parts):
    result = []
    result += process_3_face_line([parts[0], parts[1], parts[2]])
    result += process_3_face_line([parts[0], parts[2], parts[3]])
    return result

def find_index(lst, val):
    try:
        return lst.index(val)
    except ValueError:
        return -1

def create_triangle(indices, vertices, normals, uvs, newVertices, newNormals, newUvs):
    if len(newVertices) != len(newNormals) or len(newVertices) != len(newUvs) or len(newNormals) != len(newUvs):
        err.throw_error(err.ERROR_TYPE_INVALID_FILE, "big error")
        return None
    vIndex = indices[0]
    tIndex = indices[1]
    nIndex = indices[2]
    vertex = [0, 0, 0]
    if vIndex != -1:
        vertex = vertices[vIndex]
    uv = [0, 0]
    if tIndex != -1:
        uv = uvs[tIndex]
    normal = [0, 1, 0]
    if nIndex != -1:
        normal = normals[nIndex]
    vertexIndex = len(newVertices)
    newVertices.append(vertex)
    newUvs.append(uv)
    newNormals.append(normal)
    return vertexIndex

def create_new_model_lists(vertices, normals, uvs, faces):
    newVertices = []
    newUvs = []
    newNormals = []
    indices = []
    for triangle in faces:
        v0 = triangle[0]
        v1 = triangle[1]
        v2 = triangle[2]
        i0 = create_triangle(v0, vertices, normals, uvs, newVertices, newNormals, newUvs)
        i1 = create_triangle(v1, vertices, normals, uvs, newVertices, newNormals, newUvs)
        i2 = create_triangle(v2, vertices, normals, uvs, newVertices, newNormals, newUvs)
        indices.append(i0)
        indices.append(i1)
        indices.append(i2)
    return (flatten_and_to_string(newVertices), flatten_and_to_string(newNormals), flatten_and_to_string(newUvs), list_to_string(indices))

def convert_model_resource(input_file, output_file, settings_map):
    vertices = []
    normals = []
    uvs = []
    faces = []
    with open(input_file, "r") as f:
        lines = f.readlines()
        for line in lines:
            if line.startswith("v "):
                vertices.append(process_vertex_line(line))
            if line.startswith("vt "):
                uvs.append(process_uv_line(line))
            if line.startswith("vn "):
                normals.append(process_normal_line(line))
        for line in lines:
            if line.startswith("f "):
                faces += process_face_line(line)
    
    vertices, normals, uvs, indices = create_new_model_lists(vertices, normals, uvs, faces)
    writer = xml.XMLwriter(output_file)
    writer.begin("MODEL", { "name" : settings_map["Name"] })
    writer.begin("vertices", { "dim" : "3", "size" : len(vertices) })
    writer.write_text(" ".join(vertices))
    writer.end()
    writer.begin("normals", { "dim" : "3", "size" : len(normals) })
    writer.write_text(" ".join(normals))
    writer.end()
    writer.begin("uvs", { "dim" : "2", "size" : len(uvs) })
    writer.write_text(" ".join(uvs))
    writer.end()
    writer.begin("indices", { "size" : len(indices) })
    writer.write_text(" ".join(indices))
    writer.end()
    writer.end()

res.RESOURCE_HANDLERS[ft.FILE_TYPE_MODEL] = [MODEL_SETTINGS_FILE, convert_model_resource]