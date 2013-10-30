from options import SRCML_STATUS_OK

def check_return(value) :
    if value == SRCML_STATUS_OK :
        return
    raise srcMLException("Received invalid return status: " + str(value))

class srcMLException(Exception) :

    def __init__(self, message) :
        self.message = message

    def __str__(self) :
        return self.message
