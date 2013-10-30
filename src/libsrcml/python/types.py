def check_return(value) :
    if value == SRCML_STATUS_OK :
        return
    raise srcMLException("Recieved invalid return status")

class srcMLException :

    def __init__(self, message) :
        self.message = message

    def message() :
        return self.message
