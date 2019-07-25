class XMLnode:
    def __init__(self):
        self.name = ""
        self.children = []
        self.attributes = {}
        self.data = ""
        self.parent = None

    def get_child(self, name):
        for c in self.children:
            if c.name == name:
                return c
        return None

class XMLreader:
    def __init__(self, filename):
        self.filename = filename

    def load(self):
        result = XMLnode()
        f = open(self.filename, "r")
        source = f.read()
        self.process_node(source, result)
        return result.children[0]

    def process_node(self, source, parent):
        beginTagIndex = source.find("<")
        foundChildren = False
        while beginTagIndex != -1:
            beginTagEndIndex = source.find(">", beginTagIndex)
            beginTagData = source[beginTagIndex + 1 : beginTagEndIndex]
            endTagString = "</" + beginTagData + ">"
            endTagIndex = source.find(endTagString)
            if endTagIndex != -1:
                node = XMLnode()
                node.name = beginTagData
                node.parent = parent
                parent.children.append(node)
                new_source = source[beginTagEndIndex + 1 : endTagIndex]
                self.process_node(new_source, node)
                foundChildren = True
            beginTagIndex = source.find("<", beginTagEndIndex + 1)
        if not foundChildren:
            parent.data = source
            