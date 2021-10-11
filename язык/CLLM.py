class CLLM:
    def __init__(self):
        self.libs=["#include <iostream>\n", "#include <vector>\n"]
        self.included=[False, False]
        self.__globalcode=""
        self.__global_tabs=""
        self.__main_tabs=" "*4
        self.__maincode="""
int main() {

"""
    #def __add_line(self, text): self.code+=text+"\n"
    def __prepend(self): raise NotImplementedError()
    def __comment_line(self): raise NotImplementedError()
    def begin_func(self): raise NotImplementedError()
    def end_func(self): raise NotImplementedError()
    def begin_class(self): raise NotImplementedError()
    def end_class(self): raise NotImplementedError()
    def create_int_heap(self, name, val=0):
        self.__maincode += "//||--create int %s = %s--||\n" % (name, val)
        self.__maincode += self.__main_tabs + "int* %s = new int(%s);\n" % (name, val)
    def create_int(self, name, val=0):
        self.__maincode += "//||--create int %s = %s--||\n" % (name, val)
        self.__maincode += self.__main_tabs + "int %s = %s;\n" % (name, val)
    def print_int(self, name):
        if not self.included[0]:
            self.included[0]=True
            self.__globalcode+=self.libs[0]
        self.__maincode+="//||-- print int %s --||\n" % name
        self.__maincode+=self.__main_tabs + "std::cout << %s;\n"%name
    def flush(self, file):
        self.__maincode+="//RETURN\n" + self.__main_tabs+"return 0;\n}"
        with open(file, "w") as f:
            f.write(self.__globalcode+self.__maincode)