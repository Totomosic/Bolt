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

def flatten_and_to_string(lst):
    result = []
    for i in lst:
        for x in i:
            result.append(str(x))
    return result

def list_to_string(lst):
    result = []
    for i in lst:
        result.append(str(i))
    return result

def process_vertex_line(line):
    parts = line.split(" ")
    vertices = []
    for part in parts:
        try:
            num = float(part)
            vertices.append(num)
        except ValueError:
            pass
    if len(vertices) == 3:
        return [vertices[0], vertices[1], vertices[2]]
    err.throw_error(err.ERROR_TYPE_INVALID_FILE, "invalid vertex dimension")
    return None

def process_normal_line(line):
    return process_vertex_line(line)

def process_texcoord_line(line):
    parts = line.split(" ")
    texcoords = []
    for part in parts:
        try:
            num = float(part)
            texcoords.append(num)
        except ValueError:
            pass
    if len(texcoords) >= 2:
        return [texcoords[0], texcoords[1]]
    err.throw_error(err.ERROR_TYPE_INVALID_FILE, "invalid texcoord dimension")
    return None

def get_index(string):
    try:
        return int(string) - 1
    except ValueError:
        return -1

def process_face(faceVertices, outIndices, vertices, normals, texcoords, outVertices, outNormals, outTexCoords):
    outIndices.append(len(outIndices))
    outIndices.append(len(outIndices))
    outIndices.append(len(outIndices))
    for face in faceVertices:
        outVertices.append(vertices[face[0]])
        if face[1] == -1:
            outTexCoords.append([0, 0])
        else:
            outTexCoords.append(texcoords[face[1]])
        if face[2] == -1:
            outNormals.append([0, 1, 0])
        else:
            outNormals.append(normals[face[2]])

def process_face_line(line, outIndices, vertices, normals, texcoords, outVertices, outNormals, outTexCoords):
    parts = line.split(" ")[1:]
    face = []
    for part in parts:
        vtn = part.split("/")
        if len(vtn) == 3:
            face.append([get_index(vtn[0]), get_index(vtn[1]), get_index(vtn[2])])
        elif len(vtn) == 1:
            face.append([get_index(vtn[0]), -1, -1])
    if len(face) == 4:
        process_face([face[0], face[1], face[2]], outIndices, vertices, normals, texcoords, outVertices, outNormals, outTexCoords)
        process_face([face[0], face[2], face[3]], outIndices, vertices, normals, texcoords, outVertices, outNormals, outTexCoords)
    elif len(face) == 3:
        process_face([face[0], face[1], face[2]], outIndices, vertices, normals, texcoords, outVertices, outNormals, outTexCoords)

def convert_model_resource(input_file, output_file, settings_map):
    vertices = []
    normals = []
    texCoords = []
    indices = []

    realVertices = []
    realNormals = []
    realTexCoords = []
    with open(input_file, "r") as f:
        lines = f.readlines()
        for line in lines:
            if line.startswith("v "):
                vertices.append(process_vertex_line(line))
            elif line.startswith("vt "):
                texCoords.append(process_texcoord_line(line))
            elif line.startswith("vn "):
                normals.append(process_normal_line(line))
        for line in lines:
            if line.startswith("f "):
                process_face_line(line, indices, vertices, normals, texCoords, realVertices, realNormals, realTexCoords)

    writer = xml.XMLwriter(output_file)
    writer.begin("MODEL", { "name" : settings_map["Name"] })
    writer.begin("vertices", { "dim" : "3", "size" : str(len(realVertices) * 3) })
    writer.write_text(" ".join(flatten_and_to_string(realVertices)))
    writer.end()
    writer.begin("normals", { "dim" : "3", "size" : str(len(realNormals) * 3) })
    writer.write_text(" ".join(flatten_and_to_string(realNormals)))
    writer.end()
    writer.begin("texcoords", { "dim" : "2", "size" : str(len(realTexCoords) * 2) })
    writer.write_text(" ".join(flatten_and_to_string(realTexCoords)))
    writer.end()
    writer.begin("indices", { "size" : str(len(indices)) })
    writer.write_text(" ".join(list_to_string(indices)))
    writer.end()
    writer.end()

res.RESOURCE_HANDLERS[ft.FILE_TYPE_MODEL] = [MODEL_SETTINGS_FILE, convert_model_resource]