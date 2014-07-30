statementSetTagNames = set([
    "if",
    "then",
    "else",
    "while",
    "do",
    "switch",
    "case",
    "default",
    "for",
    "init",
    "incr",
    "condition",
    "break",
    "continue",
    "comment",
    "name",
    "type",
    "block",
    "index",
    "expr_stmt",
    "expr",
    "decl_stmt",
    "decl",
    "init",
    "goto",
    "label",
    "typedef",
    "asm",
    "macro",
    "empty_stmt",
    "namespace",
    "template",
    "using",
    "extern",
    "range",
    "sizeof",
    "decltype",
    "lambda"
])

functionMethodSetTagNames = set([
    "function",
    "function_decl",
    "specifier",
    "return",
    "call",
    "parameter_list",
    "param",
    "argument_list",
    "argument"
])

classSetTagNames = set([
    "class",
    "class_decl",
    "public",
    "private",
    "protected",
    "member_list",
    "constructor",
    "constructor_decl",
    "destructor",
    "destructor_decl",
    "super",
    "friend"
])

structAndUnionTagNames = set([
    "struct",
    "struct_decl",
    "union",
    "union_decl",
    "enum",
    "enum_decl"
])

exceptionTagNames = set([
    "try",
    "catch",
    "throw",
    "throws"
])

javaTagNames = set([
    "extends",
    "implements",
    "import",
    "package"
])

otherFeatureTagNames = set([
    "alignof",
    "alignas",
    "atomic",
    "generic_selection",
    "attribute"
])
extraMarkUpTagNames = set([
    "literal",
    "operator",
    "modifier"
])

# Category name constants
statementCategory = "Statements" 
functionsAndMethodsCategory = "Functions/Methods"
classCategory = "Class"
structUnionAndEnumCategory = "Structs, Unions and Enums"
exceptionCategory = "Exceptions"
javaCategory = "Java"
extraMarkupCategory = "Extra Markup"
otherFeaturesCategory = "Other Language Elements"
cpreprocCategory = "C Preprocessor"
unknownCategory = "Unknown FIXME!"


categoryOrdering = [
    statementCategory,
    functionsAndMethodsCategory,
    classCategory,
    structUnionAndEnumCategory,
    exceptionCategory,
    javaCategory,
    extraMarkupCategory,
    cpreprocCategory,
    otherFeaturesCategory,
    unknownCategory
]

def getCategoryFromElement(elem):
    if elem.ns.lower() == "cpp":
        return cpreprocCategory
    elif elem.tag in statementSetTagNames:
        return statementCategory
    elif elem.tag in functionMethodSetTagNames:
        return functionsAndMethodsCategory
    elif elem.tag in classSetTagNames:
        return classCategory
    elif elem.tag in structAndUnionTagNames:
        return structUnionAndEnumCategory
    elif elem.tag in exceptionTagNames:
        return exceptionCategory
    elif elem.tag in javaTagNames:
        return javaCategory
    elif elem.tag in otherFeatureTagNames:
        return otherFeaturesCategory
    elif elem.ns == "op" or elem.ns == "lit" or elem.ns == "type":
        return extraMarkupCategory
    else:
        return unknownCategory

def translateToProperCategory(dataCat):
    if dataCat.lower() == "statement" or dataCat.lower() == "stmt":
        return statementCategory
    elif dataCat.lower() == "func":
        return functionsAndMethodsCategory
    elif dataCat.lower() == "struct" or  dataCat.lower() == "union" or dataCat.lower() == "enum":
        return structUnionAndEnumCategory
    elif dataCat.lower() == "cls":
        return classCategory
    elif dataCat.lower() == "java":
        return javaCategory
    elif dataCat.lower() == "extra":
        return extraMarkupCategory
    elif dataCat == "other":
        return otherFeaturesCategory
    elif (dataCat == statementCategory or
            dataCat == functionsAndMethodsCategory or
            dataCat == structUnionAndEnumCategory or
            dataCat == exceptionCategory or
            dataCat == javaCategory or
            dataCat == extraMarkupCategory or
            dataCat == otherFeaturesCategory):
        return datacat
    else:
        print 'Unknown category: ', dataCat
        return unknownCategory





#  Newest ordering categories


# New Categories
# Statements
# Definition/Declaration
# Expression

statementTagSet = set([
    "if",
    "then",
    "else",
    "while",
    "do",
    "switch",
    "case",
    "default",
    "for",    
    "break",
    "continue",
    "comment",
    "block",
    "index",
    "expr_stmt",
    "goto",
    "label",
    "typedef",
    "asm",
    "macro",
    "empty_stmt"

])

defDeclTagSet = set([
    "using",
    "namespace",
    "extern",
    "decl_stmt",
    "decl",
    "init",
    "lambda",
    "range",
    "template",
    "type",
    "function",
    "function_decl",
    "specifier",
    "return",
    "call",
    "parameter_list",
    "param",
    "argument_list",
    "argument",
    "class",
    "class_decl",
    "public",
    "private",
    "protected",
    "member_list",
    "constructor",
    "constructor_decl",
    "destructor",
    "destructor_decl",
    "super",
    "friend",
    "struct",
    "struct_decl",
    "union",
    "union_decl",
    "enum",
    "enum_decl",
    "try",
    "catch",
    "throw",
    "throws"
])

exprTagSet = set([
    "expr",
    "sizeof",
    "decltype",
    "condition",
    "name"
])

languageSpecificTagSet = set([
    "alignof",
    "alignas",
    "atomic",
    "generic_selection",
    "attribute",
    "extends",
    "implements",
    "import",
    "package"
])

# statementSetTagNames = set([
#     "if",
#     "then",
#     "else",
#     "while",
#     "do",
#     "switch",
#     "case",
#     "default",
#     "for",
#     "init",
#     "incr",
#     "condition",
#     "break",
#     "continue",
#     "comment",
#     "name",
#     "type",
#     "block",
#     "index",
#     "expr_stmt",
#     "expr",
#     "decl_stmt",
#     "decl",
#     "init",
#     "goto",
#     "label",
#     "typedef",
#     "asm",
#     "macro",
#     "empty_stmt",
#     "namespace",
#     "template",
#     "using",
#     "extern",
#     "range",
#     "sizeof",
#     "decltype",
#     "lambda"
# ])

# functionMethodSetTagNames = set([
#     "function",
#     "function_decl",
#     "specifier",
#     "return",
#     "call",
#     "parameter_list",
#     "param",
#     "argument_list",
#     "argument"
# ])

# classSetTagNames = set([
#     "class",
#     "class_decl",
#     "public",
#     "private",
#     "protected",
#     "member_list",
#     "constructor",
#     "constructor_decl",
#     "destructor",
#     "destructor_decl",
#     "super",
#     "friend"
# ])

# structAndUnionTagNames = set([
#     "struct",
#     "struct_decl",
#     "union",
#     "union_decl",
#     "enum",
#     "enum_decl"
# ])

# exceptionTagNames = set([
#     "try",
#     "catch",
#     "throw",
#     "throws"
# ])

# javaTagNames = set([
#     "extends",
#     "implements",
#     "import",
#     "package"
# ])

# otherFeatureTagNames = set([
#     "alignof",
#     "alignas",
#     "atomic",
#     "generic_selection",
#     "attribute"
# ])

# extraMarkUpTagNames = set([
#     "literal",
#     "operator",
#     "modifier"
# ])

# Old Category Names
# Category name constants
# statementCategory = "Statements" 
# functionsAndMethodsCategory = "Functions/Methods"
# classCategory = "Class"
# structUnionAndEnumCategory = "Structs, Unions and Enums"
# exceptionCategory = "Exceptions"
# javaCategory = "Java"
# extraMarkupCategory = "Extra Markup"
# otherFeaturesCategory = "Other Language Elements"
# cpreprocCategory = "C Preprocessor"
# unknownCategory = "Unknown FIXME!"


statementCategory = "Statements"
defDeclCategory = "Definitions/Declarations"
expressionsCategory = "Expressions"
languageCategory = "Language Specific"
cpreprocCategory = "C Preprocessor"
unknownCategory = "Unknown FIXME!"

categoryOrdering = [
    statementCategory,
    defDeclCategory,
    expressionsCategory,
    languageCategory,
    cpreprocCategory,
    unknownCategory
]

# Old category Listings
# categoryOrdering = [
#     statementCategory,
#     functionsAndMethodsCategory,
#     classCategory,
#     structUnionAndEnumCategory,
#     exceptionCategory,
#     javaCategory,
#     extraMarkupCategory,
#     cpreprocCategory,
#     otherFeaturesCategory,
#     unknownCategory
# ]

def getCategoryFromElement(elem):
    # if elem.ns.lower() == "cpp":
    #     return cpreprocCategory
    # elif elem.tag in statementSetTagNames:
    #     return statementCategory
    # elif elem.tag in functionMethodSetTagNames:
    #     return functionsAndMethodsCategory
    # elif elem.tag in classSetTagNames:
    #     return classCategory
    # elif elem.tag in structAndUnionTagNames:
    #     return structUnionAndEnumCategory
    # elif elem.tag in exceptionTagNames:
    #     return exceptionCategory
    # elif elem.tag in javaTagNames:
    #     return javaCategory
    # elif elem.tag in otherFeatureTagNames:
    #     return otherFeaturesCategory
    # elif elem.ns == "op" or elem.ns == "lit" or elem.ns == "type":
    #     return extraMarkupCategory
    # else:
    #     return unknownCategory

    if elem.ns.lower() == "cpp":
        return cpreprocCategory
    
    elif elem.tag in statementTagSet:
        return statementCategory
    
    elif elem.tag in defDeclTagSet:
        return defDeclCategory
    
    elif elem.tag in exprTagSet:
        return expressionsCategory

    elif elem.tag in languageSpecificTagSet:
        return languageCategory
    
    else:
        return unknownCategory

def translateToProperCategory(dataCat):
    # if dataCat.lower() == "statement" or dataCat.lower() == "stmt":
    #     return statementCategory
    # elif dataCat.lower() == "func":
    #     return functionsAndMethodsCategory
    # elif dataCat.lower() == "struct" or  dataCat.lower() == "union" or dataCat.lower() == "enum":
    #     return structUnionAndEnumCategory
    # elif dataCat.lower() == "cls":
    #     return classCategory
    # elif dataCat.lower() == "java":
    #     return javaCategory
    # elif dataCat.lower() == "extra":
    #     return extraMarkupCategory
    # elif dataCat == "other":
    #     return otherFeaturesCategory
    # elif (dataCat == statementCategory or
    #         dataCat == functionsAndMethodsCategory or
    #         dataCat == structUnionAndEnumCategory or
    #         dataCat == exceptionCategory or
    #         dataCat == javaCategory or
    #         dataCat == extraMarkupCategory or
    #         dataCat == otherFeaturesCategory):
    #     return datacat
    # else:
    #     print 'Unknown category: ', dataCat
    #     return unknownCategory

    if dataCat.lower() == "cpp":
        return cpreprocCategory
    elif dataCat.lower() == "stmt":
        return statementCategory
    elif dataCat.lower() == "def" or dataCat.lower() == "decl":
        return defDeclCategory
    elif dataCat.lower() == "expr":
        return expressionsCategory
    elif dataCat.lower() == "lang":
        return languageCategory
    elif dataCat in categoryOrdering:
        return dataCat
    else:
        print 'Unknown category: ', dataCat
        return unknownCategory
