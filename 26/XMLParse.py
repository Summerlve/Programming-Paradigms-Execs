from xml.sax import make_parser, ContentHandler
from urllib2 import urlopen

class XMLContentHandler(ContentHandler):
    def __init__(self):
        ContentHandler.__init__(self)
        self.__inItem = False
        self.__inTitle = False
    
    def startElement(self, tag, attrs):
        if tag == "item": self.__inItem = True
        if tag == "title" and self.__inItem:
            self.__inTitle = True

    def endElement(self, tag):
        if tag == "item": self.__inItem = False
        if tag == "title" and self.__inTitle:
            self.__inTitle = False

    def characters(self, data):
        if self.__inItem and self.__inTitle:
            print data

class UrlContentHandler(ContentHandler):
    def __init__(self):
        ContentHandler.__init__(self)
        self.__isInTag = False

    def startElement(self, tag, attrs):
        if tag: self.__isInTag = True
    
    def endElement(self, tag):
        if tag: self.__isInTag = False

    def characters(self, data):
        if self.__isInTag: print data


if __name__ == "__main__":
    infile = open("./example.xml")
    parser = make_parser()
    parser.setContentHandler(XMLContentHandler())
    parser.parse(infile)
    infile.close()

    urlFile = urlopen("http://www.w3school.com.cn/example/xmle/note.xml")
    urlparser = make_parser()
    urlparser.setContentHandler(UrlContentHandler())
    urlparser.parse(urlFile)