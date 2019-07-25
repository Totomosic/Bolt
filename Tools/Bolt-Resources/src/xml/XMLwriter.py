OPEN_MODE_TEXT = 0
OPEN_MODE_BYTES = 1

class XMLwriter:
    def __init__(self, filename):
        self.filename = filename
        self.file = open(filename, "w")
        self.current_tags = []
        self.open_mode = OPEN_MODE_TEXT
        self.added_data = True

    def current_tag_count(self):
        return len(self.current_tags)

    def open_file(self, mode):
        if mode != self.open_mode:
            self.open_mode = mode
            self.file.close()
            if mode == OPEN_MODE_TEXT:
                self.file = open(self.filename, "a")
            else:
                self.file = open(self.filename, "ab")

    def begin(self, name, attributes = {}):
        self.open_file(OPEN_MODE_TEXT)
        if not self.added_data:
            self.file.write("\n")
        for i in range(self.current_tag_count()):
            self.file.write('\t')
        self.file.write("<{}".format(name))
        for attrib in attributes:
            self.file.write(' {}="{}"'.format(attrib, attributes[attrib]))
        self.file.write(">")
        self.current_tags.append(name)
        self.added_data = False

    def end(self):
        current_tag = self.current_tags[-1]
        self.current_tags.pop()
        self.open_file(OPEN_MODE_TEXT)
        self.file.write("</{}>\n".format(current_tag))
        if len(self.current_tags) == 0:
            self.file.close()
        self.added_data = True

    def write_text(self, text):
        self.open_file(OPEN_MODE_TEXT)
        self.file.write(text)
        self.added_data = True

    def write_bytes(self, byte_arr):
        self.open_file(OPEN_MODE_BYTES)
        self.file.write(byte_arr)
        self.added_data = True