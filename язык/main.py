import os, re, subprocess
import CLLM
#PRIVATE
enum_count=-1
#PRIVATE
def n_enm(b=False):
    global enum_count
    if b:
        enum_count=-1
    enum_count+=1
    return enum_count
TOKEN_IDENTIFIER=n_enm()
TOKEN_DEF=n_enm()
TOKEN_UNKNOWN=n_enm()
TOKEN_PLUS=n_enm()
TOKEN_MINUS=n_enm()
TOKEN_NUMBER=n_enm()
TOKEN_EOF=n_enm()

NO_ERROR=n_enm(True)
UNKNOWN_TOKEN_ERROR=n_enm()
from os import listdir
from os.path import isfile, join
class Token:
    def __init__(self, type, value=None):
        self.type=type
        self.value=value
    def __repr__(self):
        return str(self)
    def __str__(self):
        if self.type==TOKEN_IDENTIFIER:
            return "Token identifier: "+str(self.value)
        elif self.type==TOKEN_DEF:
            return "Token def: "
        elif self.type==TOKEN_NUMBER:
            return "Token number: "+str(self.value)
        elif self.type==TOKEN_EOF:
            return "Token: EOF"
        elif self.type==TOKEN_PLUS:
            return "Token plus: "
        elif self.type== TOKEN_UNKNOWN:
            return "Token unknown: "+str(self.value)
        return "Token??"
class Lexer:
    def __nextchr(self):
        if (self.index==len(self.text)): self.token=''
        else:self.token=self.text[self.index];self.index+=1
    def check_identifier(self, k):
        for key, item in self.identifiers.items():
            if key == k:
                return Token(item, k)
        return Token(TOKEN_IDENTIFIER, k)
    def __init__(self, text):
        self.index=0
        self.token=" "
        self.tokens=[]
        self.text=" "+text
        self.single_tokens={
            "+":TOKEN_PLUS,
            "-":TOKEN_MINUS
        }
        self.identifiers={
            "def":TOKEN_DEF
        }
    def Next(self):
        #SPACES
        while self.token==' ' or self.token=='\n': self.__nextchr()
        if self.token=='': return Token(TOKEN_EOF,)
        k=""
        if self.token.isalpha()or self.token=='_':
            k+=self.token
            self.__nextchr()
            while self.token.isalnum() or self.token=='_':
                k+=self.token
                self.__nextchr()
            return self.check_identifier(k)
        while self.token.isdigit():
            k+=self.token
            self.__nextchr()
        if k: return Token(TOKEN_NUMBER, k)
        l=self.token[:]
        self.__nextchr()
        return Token(TOKEN_UNKNOWN,)
class NodeAST:
    def __init__(self):
        pass
class DefExpr(NodeAST):
    def __init__(self):
        self.__init__()


class Parser:
    def __init__(self, text):
        self.lex=Lexer(text)
        self.coder=CLLM.CLLM()
    def parse_def(self):
        tok=self.lex.Next()
        if tok.type==TOKEN_IDENTIFIER:
            tok=self.lex.Next()
            if tok.type==TOKEN_UNKNOWN and tok.value=="+":
                #GEN CODE
                print("Parsed func def")
                return 0
    def parse(self):
        tok=self.lex.Next()
        while tok.type!=TOKEN_EOF:
            if tok.type==TOKEN_UNKNOWN: return UNKNOWN_TOKEN_ERROR
            if tok.type==TOKEN_IDENTIFIER: return self.__parse_identifier()
            print(tok)
            tok=self.lex.Next()
        return 0

#subprocess.run(["chcp 65001 "], shell=True, stdout=subprocess.PIPE)
#C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat
if __name__=="__main__":
    p=Parser("7878 78787 778787 ")
    p.parse()

