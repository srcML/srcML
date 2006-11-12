java antlr.Tool WSLexer.g
java antlr.Tool -glib WSLexer.g CommentLexer.g
java antlr.Tool -glib WSLexer.g;CommentLexer.g OperatorLexer.g
java antlr.Tool -glib WSLexer.g;CommentLexer.g;OperatorLexer.g TextLexer.g
java antlr.Tool -glib WSLexer.g;CommentLexer.g;OperatorLexer.g;TextLexer.g KeywordLexer.g
java antlr.Tool -glib WSLexer.g;CommentLexer.g;OperatorLexer.g;TextLexer.g;KeywordLexer.g KeywordCPPLexer.g
java antlr.Tool srcMLparser.g
