
from pygments.style import Style
from pygments.token import Keyword, Name, Comment, String, Error, \
     Number, Operator, Generic

class RhobanStyle(Style):
    default_style = ""
    
    styles = {
        Comment:                'italic #888',
        Keyword:                'bold #005',
        Name:                   '#003',
        Name.Function:          'bold #003',
        Name.Class:             'bold #0f0',
        String:                 '#c11',
        Number:                 '#c11',
        Generic:                '#0f0'
    }

