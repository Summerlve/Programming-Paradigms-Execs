#-*- coding:utf-8 -*-
import urllib2
import string

def getFavouriteWord(urls):
    """
    urls: [], collections of many urls such as: ["url1", "url2", ...]
    words = {
        "word_a": {
            freq: 100,
            len: 5,
            score: 100 ^ 5
        },
        "word_b": {
            freq: 200,
            len: 6,
            score: 200 ^ 6
        }
    }
    """
    # count all word in txt
    words = {}
    for url in urls: 
        contents = getUrlContentInTXT(url).splitlines() # get content in txt and split into serval lines
        for line in contents:
            # remove all punctuations in line
            lineRemovePunctuation = line.translate(None, string.punctuation)
            wordsInLine = lineRemovePunctuation.split(" ")
            for word in wordsInLine:
                if not word.islower():
                    continue
                if word in words:
                    words[word]["freq"] += 1
                else:
                    words[word] = {
                        "freq": 1,
                        "len": len(word),
                        "score": 0
                    }

    favourite = ""
    topScore = 0

    # eval score of all words and cal out favourite word
    for word, data in words.iteritems():
        score = data["score"] = data["freq"] ** data["len"]
        if score > topScore:
            topScore = score
            favourite = word

    return {
        favourite: words[favourite]
    }
    
# download txt
def getUrlContentInTXT(url):
    content = urllib2.urlopen(url).read()
    return content
    
# http://www.gutenberg.org/
print getFavouriteWord(["http://www.gutenberg.org/cache/epub/5200/pg5200.txt",
                        "http://www.gutenberg.org/cache/epub/7849/pg7849.txt"])