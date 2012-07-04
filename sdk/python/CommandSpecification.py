from xml.dom import minidom

pathToXml = "/home/baudouin/Stage/Edu/sdk/common/commands.xml"
xmldoc = minidom.parse(pathToXml)

class CommandSpecification:
    
    def __init__(self,name,description,destination,index,parametersPattern,answerPattern):
        self.name = "Name"
        self.descritption = "Description"
        self.destination = "Destination"
        self.index = "CommandIndex"
        self.parametersPattern = "ParametersPattern"
        self.answerPattern = "AnswerPattern"

class CommandStore:
    
    def __init__(self,xmldoc):
        self.store = {}
        for command in xmldoc.childNodes[0].getElementsByTagName("CommandSpecification"):
            name = command.getElementsByTagName("Name")[0].childNodes[0].data
            description = command.getElementsByTagName("Description")[0].childNodes[0].data
            destination = command.getElementsByTagName("Destination")[0].childNodes[0].data
            commandIndex = command.getElementsByTagName("CommandIndex")[0].childNodes[0].data

            if command.getElementsByTagName("ParametersPattern")[0].childNodes==[]:
                parametersPattern = ""
            else :
                parametersPattern = command.getElementsByTagName("ParametersPattern")[0].childNodes[0].data
            if command.getElementsByTagName("AnswerPattern")[0].childNodes==[]:
                answerPattern = ""
            else:
                answerPattern = command.getElementsByTagName("AnswerPattern")[0].childNodes[0].data

            self.store[name] = CommandSpecification(name, description, destination, commandIndex, parametersPattern, answerPattern)

    def get(name,store):
        return(store[name])

if __name__ == '__main__':
    dictionnary = CommandStore(xmldoc)
    print(dictionnary)

def test():
    dictionnary = CommandStore(xmldoc)
    print(dictionnary)




            
            
