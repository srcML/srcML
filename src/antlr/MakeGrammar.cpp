// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file MakeGrammar.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.DefineGrammarSymbols and antlr.MakeGrammar.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "MakeGrammar.hpp"
#include "LLkAnalyzer.hpp"
#include "Tool.hpp"

namespace antlrcpp {

namespace {
    const std::string DEFAULT_TOKENMANAGER_NAME = "*default";
}

void TreeBlockContext::addAlternativeElement(AlternativeElement* e) {
    auto tree = static_cast<TreeElement*>(block);
    if (nextElementIsRoot) {
        tree->root = static_cast<GrammarAtom*>(e);
        nextElementIsRoot = false;
    } else {
        BlockContext::addAlternativeElement(e);
    }
}

MakeGrammar::MakeGrammar(Tool* tool, std::vector<std::string> args, LLkAnalyzer* analyzer)
    : tool(tool), analyzer(analyzer), args(std::move(args)) {}

// DefineGrammarSymbols

void MakeGrammar::_refStringLiteral(const TokenRef& lit) {
    if (!grammar->isLexer()) {
        const std::string& str = lit->text;
        if (grammar->tokenManager->getTokenSymbol(str))
            return;
        auto sl = new StringLiteralSymbol(str);
        int tt = grammar->tokenManager->nextTokenType();
        sl->setTokenType(tt);
        grammar->tokenManager->define(sl);
    }
}

void MakeGrammar::_refToken(const TokenRef& t) {
    const std::string& id = t->text;
    if (!grammar->tokenManager->tokenDefined(id)) {
        int tt = grammar->tokenManager->nextTokenType();
        auto ts = new TokenSymbol(id);
        ts->setTokenType(tt);
        grammar->tokenManager->define(ts);
    }
}

void MakeGrammar::abortGrammar() {
    std::string s = "unknown grammar";
    if (grammar)
        s = grammar->getClassName();
    tool->error("aborting grammar '" + s + "' due to errors");
    if (grammar)
        grammars.remove(grammar->getClassName());
    grammar = nullptr;
}

void MakeGrammar::defineToken(const TokenRef& tokname, const TokenRef& tokliteral) {
    OStr name;
    OStr literal;
    if (tokname)
        name = tokname->text;
    if (tokliteral)
        literal = tokliteral->text;
    TokenManager* tm = grammar->tokenManager;
    if (literal) {
        auto sl = dynamic_cast<StringLiteralSymbol*>(tm->getTokenSymbol(*literal));
        if (sl) {
            if (!name || sl->label) {
                tool->warning("Redefinition of literal in tokens {...}: " + *literal, grammar->getFilename(), tokliteral->line, tokliteral->column);
                return;
            } else if (name) {
                sl->label = *name;
                tm->mapToTokenSymbol(*name, sl);
            }
        }
        if (name) {
            TokenSymbol* ts = tm->getTokenSymbol(*name);
            if (ts) {
                if (dynamic_cast<StringLiteralSymbol*>(ts)) {
                    tool->warning("Redefinition of token in tokens {...}: " + *name, grammar->getFilename(), tokliteral->line, tokliteral->column);
                    return;
                }
                int ttype = ts->getTokenType();
                sl = new StringLiteralSymbol(*literal);
                sl->setTokenType(ttype);
                sl->label = *name;
                tm->define(sl);
                tm->mapToTokenSymbol(*name, sl);
                return;
            }
        }
        sl = new StringLiteralSymbol(*literal);
        int tt = tm->nextTokenType();
        sl->setTokenType(tt);
        sl->label = name;
        tm->define(sl);
        if (name)
            tm->mapToTokenSymbol(*name, sl);
    } else {
        if (tm->tokenDefined(*name)) {
            tool->warning("Redefinition of token in tokens {...}: " + *name, grammar->getFilename(), tokname->line, tokname->column);
            return;
        }
        int tt = tm->nextTokenType();
        auto ts = new TokenSymbol(*name);
        ts->setTokenType(tt);
        tm->define(ts);
    }
}

void MakeGrammar::endOptions() {
    if (!grammar->exportVocab && !grammar->importVocab) {
        grammar->exportVocab = grammar->getClassName();
        if (tokenManagers.containsKey(DEFAULT_TOKENMANAGER_NAME)) {
            grammar->exportVocab = DEFAULT_TOKENMANAGER_NAME;
            grammar->tokenManager = *tokenManagers.get(DEFAULT_TOKENMANAGER_NAME);
            return;
        }
        auto tm = new TokenManager(*grammar->exportVocab, tool);
        grammar->tokenManager = tm;
        tokenManagers.put(*grammar->exportVocab, tm);
        tokenManagers.put(DEFAULT_TOKENMANAGER_NAME, tm);
        return;
    }
    if (!grammar->exportVocab && grammar->importVocab) {
        grammar->exportVocab = grammar->getClassName();
        if (*grammar->importVocab == *grammar->exportVocab) {
            tool->warning("Grammar " + grammar->getClassName() + " cannot have importVocab same as default output vocab (grammar name); ignored.");
            grammar->importVocab.reset();
            endOptions();
            return;
        }
        if (tokenManagers.containsKey(*grammar->importVocab)) {
            TokenManager* dup = (*tokenManagers.get(*grammar->importVocab))->clone();
            dup->setName(*grammar->exportVocab);
            dup->setReadOnly(false);
            grammar->tokenManager = dup;
            tokenManagers.put(*grammar->exportVocab, dup);
            return;
        }
        auto tm = new TokenManager(*grammar->exportVocab, tool);
        tm->loadVocabFile(*grammar->importVocab + "TokenTypes.txt");
        tm->setReadOnly(false);
        tokenManagers.put(*grammar->exportVocab, tm);
        grammar->tokenManager = tm;
        if (!tokenManagers.containsKey(DEFAULT_TOKENMANAGER_NAME))
            tokenManagers.put(DEFAULT_TOKENMANAGER_NAME, tm);
        return;
    }
    if (grammar->exportVocab && !grammar->importVocab) {
        if (tokenManagers.containsKey(*grammar->exportVocab)) {
            grammar->tokenManager = *tokenManagers.get(*grammar->exportVocab);
            return;
        }
        auto tm = new TokenManager(*grammar->exportVocab, tool);
        grammar->tokenManager = tm;
        tokenManagers.put(*grammar->exportVocab, tm);
        if (!tokenManagers.containsKey(DEFAULT_TOKENMANAGER_NAME))
            tokenManagers.put(DEFAULT_TOKENMANAGER_NAME, tm);
        return;
    }
    if (grammar->exportVocab && grammar->importVocab) {
        if (*grammar->importVocab == *grammar->exportVocab)
            tool->error("exportVocab of " + *grammar->exportVocab + " same as importVocab; probably not what you want");
        if (tokenManagers.containsKey(*grammar->importVocab)) {
            TokenManager* dup = (*tokenManagers.get(*grammar->importVocab))->clone();
            dup->setName(*grammar->exportVocab);
            dup->setReadOnly(false);
            grammar->tokenManager = dup;
            tokenManagers.put(*grammar->exportVocab, dup);
            return;
        }
        auto tm = new TokenManager(*grammar->exportVocab, tool);
        tm->loadVocabFile(*grammar->importVocab + "TokenTypes.txt");
        tm->setReadOnly(false);
        tokenManagers.put(*grammar->exportVocab, tm);
        grammar->tokenManager = tm;
        if (!tokenManagers.containsKey(DEFAULT_TOKENMANAGER_NAME))
            tokenManagers.put(DEFAULT_TOKENMANAGER_NAME, tm);
        return;
    }
}

void MakeGrammar::refHeaderAction(const TokenRef& name, const TokenRef& act) {
    std::string key = name ? stripFrontBack(name->text, "\"", "\"") : std::string();
    if (headerActions.containsKey(key)) {
        if (key.empty())
            tool->error(std::to_string(act->line) + ": header action already defined");
        else
            tool->error(std::to_string(act->line) + ": header action '" + key + "' already defined");
    }
    headerActions.put(key, act);
}

void MakeGrammar::setCharVocabulary(const BitSet& b) {
    static_cast<LexerGrammar*>(grammar)->setCharVocabulary(b);
}

void MakeGrammar::setFileOption(const TokenRef& key, const TokenRef& value, const std::string& filename) {
    const std::string& k = key->text;
    if (k == "language") {
        if (value->type == ANTLR_STRING_LITERAL)
            language = stripBack(stripFront(value->text, '"'), '"');
        else if (value->type == ANTLR_TOKEN_REF || value->type == ANTLR_RULE_REF)
            language = value->text;
        else
            tool->error("language option must be string or identifier", filename, value->line, value->column);
    } else if (k == "mangleLiteralPrefix") {
        if (value->type == ANTLR_STRING_LITERAL)
            tool->literalsPrefix = stripFrontBack(value->text, "\"", "\"");
        else
            tool->error("mangleLiteralPrefix option must be string", filename, value->line, value->column);
    } else if (k == "upperCaseMangledLiterals") {
        if (value->text == "true")
            tool->upperCaseMangledLiterals = true;
        else if (value->text == "false")
            tool->upperCaseMangledLiterals = false;
        else
            tool->error("Value for upperCaseMangledLiterals must be true or false", filename, key->line, key->column);
    } else if (k == "namespaceStd" || k == "namespaceAntlr" || k == "genHashLines") {
        if (language != "Cpp") {
            tool->error(k + " option only valid for C++", filename, key->line, key->column);
        } else if (k == "genHashLines") {
            if (!(value->text == "true" || value->text == "false"))
                tool->error("genHashLines option must be true or false", filename, value->line, value->column);
            tool->genHashLines = value->text == "true";
        } else if (value->type != ANTLR_STRING_LITERAL) {
            tool->error(k + " option must be a string", filename, value->line, value->column);
        } else if (k == "namespaceStd") {
            tool->namespaceStd = value->text;
        } else {
            tool->namespaceAntlr = value->text;
        }
    } else if (k == "namespace") {
        if (language != "Cpp" && language != "CSharp")
            tool->error(k + " option only valid for C++ and C# (a.k.a CSharp)", filename, key->line, key->column);
        else if (value->type != ANTLR_STRING_LITERAL)
            tool->error(k + " option must be a string", filename, value->line, value->column);
        else
            tool->setNameSpace(value->text);
    } else {
        tool->error("Invalid file-level option: " + k, filename, key->line, value->column);
    }
}

void MakeGrammar::setGrammarOption(const TokenRef& key, const TokenRef& value) {
    const std::string& k = key->text;
    if (k == "tokdef" || k == "tokenVocabulary") {
        tool->error("tokdef/tokenVocabulary options are invalid >= ANTLR 2.6.0.\n"
                    "  Use importVocab/exportVocab instead.  Please see the documentation.\n"
                    "  The previous options were so heinous that Terence changed the whole\n"
                    "  vocabulary mechanism; it was better to change the names rather than\n"
                    "  subtly change the functionality of the known options.  Sorry!",
                    grammar->getFilename(), value->line, value->column);
    } else if (k == "literal" && grammar->isLexer()) {
        tool->error("the literal option is invalid >= ANTLR 2.6.0.\n"
                    "  Use the \"tokens {...}\" mechanism instead.",
                    grammar->getFilename(), value->line, value->column);
    } else if (k == "exportVocab") {
        if (value->type == ANTLR_RULE_REF || value->type == ANTLR_TOKEN_REF)
            grammar->exportVocab = value->text;
        else
            tool->error("exportVocab must be an identifier", grammar->getFilename(), value->line, value->column);
    } else if (k == "importVocab") {
        if (value->type == ANTLR_RULE_REF || value->type == ANTLR_TOKEN_REF)
            grammar->importVocab = value->text;
        else
            tool->error("importVocab must be an identifier", grammar->getFilename(), value->line, value->column);
    } else if (k == "k") {
        if (grammar->isTreeWalker() && value->text != "1")
            tool->error("Treewalkers only support k=1", grammar->getFilename(), value->line, value->column);
        else
            grammar->setOption(k, value);
    } else {
        grammar->setOption(k, value);
    }
}

void MakeGrammar::startLexer(const std::string& file, const TokenRef& name, const OStr& superClass, const OStr& doc) {
    if (numLexers > 0)
        tool->panic("You may only have one lexer per grammar file: class " + name->text);
    numLexers++;
    reset();
    auto lg = new LexerGrammar(name->text, tool, superClass);
    lg->comment = doc;
    lg->processArguments(args);
    lg->setFilename(file);
    grammars.put(lg->getClassName(), lg);
    lg->preambleAction = thePreambleAction;
    thePreambleAction = makeTokenRef(TokenType::INVALID_TYPE, "");
    grammar = lg;
}

void MakeGrammar::startParser(const std::string& file, const TokenRef& name, const OStr& superClass, const OStr& doc) {
    if (numParsers > 0)
        tool->panic("You may only have one parser per grammar file: class " + name->text);
    numParsers++;
    reset();
    grammar = new ParserGrammar(name->text, tool, superClass);
    grammar->comment = doc;
    grammar->processArguments(args);
    grammar->setFilename(file);
    grammars.put(grammar->getClassName(), grammar);
    grammar->preambleAction = thePreambleAction;
    thePreambleAction = makeTokenRef(TokenType::INVALID_TYPE, "");
}

void MakeGrammar::startTreeWalker(const std::string& file, const TokenRef& name, const OStr& superClass, const OStr& doc) {
    if (numTreeParsers > 0)
        tool->panic("You may only have one tree parser per grammar file: class " + name->text);
    numTreeParsers++;
    reset();
    grammar = new TreeWalkerGrammar(name->text, tool, superClass);
    grammar->comment = doc;
    grammar->processArguments(args);
    grammar->setFilename(file);
    grammars.put(grammar->getClassName(), grammar);
    grammar->preambleAction = thePreambleAction;
    thePreambleAction = makeTokenRef(TokenType::INVALID_TYPE, "");
}

// MakeGrammar

void MakeGrammar::addElementToCurrentAlt(AlternativeElement* e) {
    e->enclosingRuleName = ruleBlock->ruleName;
    context()->addAlternativeElement(e);
}

void MakeGrammar::beginAlt(bool doAutoGen) {
    auto alt = new Alternative();
    alt->setAutoGen(doAutoGen);
    context()->block->addAlternative(alt);
}

void MakeGrammar::beginChildList() {
    context()->block->addAlternative(new Alternative());
}

void MakeGrammar::beginExceptionGroup() {
    if (!dynamic_cast<RuleBlock*>(context()->block))
        tool->panic("beginExceptionGroup called outside of rule block");
}

void MakeGrammar::beginExceptionSpec(const TokenRef& label) {
    if (label)
        label->text = stripFront(stripBack(label->text, std::string(" \n\r\t")), std::string(" \n\r\t"));
    currentExceptionSpec = new ExceptionSpec(label);
}

void MakeGrammar::beginSubRule(const TokenRef& label, const TokenRef& start, bool not_) {
    blocks.push_back(new BlockContext());
    context()->block = new AlternativeBlock(grammar, start, not_);
    context()->altNum = 0;
    nested++;
    context()->blockEnd = new BlockEndElement(grammar);
    context()->blockEnd->block = context()->block;
    labelElement(context()->block, label);
}

void MakeGrammar::beginTree(const TokenRef& tok) {
    if (!grammar->isTreeWalker()) {
        tool->error("Trees only allowed in TreeParser", grammar->getFilename(), tok->line, tok->column);
        throw RecognitionException{ "Trees only allowed in TreeParser", grammar->getFilename(), tok->line, tok->column };
    }
    blocks.push_back(new TreeBlockContext());
    context()->block = new TreeElement(grammar, tok);
    context()->altNum = 0;
}

RuleBlock* MakeGrammar::createNextTokenRule(Grammar* g, const std::vector<RuleSymbol*>& lexRules, const std::string& rname) {
    auto rb = new RuleBlock(g, rname);
    rb->setDefaultErrorHandler(g->getDefaultErrorHandler());
    auto ruleEnd = new RuleEndElement(g);
    rb->setEndElement(ruleEnd);
    ruleEnd->block = rb;
    for (auto r : lexRules) {
        if (!r->isDefined()) {
            g->antlrTool->error("Lexer rule " + r->id.substr(1) + " is not defined");
        } else if (r->access == "public") {
            auto alt = new Alternative();
            RuleBlock* targetRuleBlock = r->getBlock();
            auto& targetRuleAlts = targetRuleBlock->getAlternatives();
            if (targetRuleAlts.size() == 1) {
                Alternative* onlyAlt = targetRuleAlts[0];
                if (onlyAlt->semPred)
                    alt->semPred = onlyAlt->semPred;
            }
            auto rr = new RuleRefElement(g, makeTokenRef(ANTLR_RULE_REF, r->getId()), AUTO_GEN_NONE);
            rr->setLabel("theRetToken");
            rr->enclosingRuleName = "nextToken";
            rr->next = ruleEnd;
            alt->addElement(rr);
            alt->setAutoGen(true);
            rb->addAlternative(alt);
            r->addReference(rr);
        }
    }
    rb->setAutoGen(true);
    rb->prepareForAnalysis();
    return rb;
}

AlternativeBlock* MakeGrammar::createOptionalRuleRef(const std::string& rule, const TokenRef& start) {
    auto blk = new AlternativeBlock(grammar, start, false);
    std::string mrule = encodeLexerRuleName(rule);
    if (!grammar->isDefined(mrule))
        grammar->define(new RuleSymbol(mrule));
    auto t = makeTokenRef(ANTLR_TOKEN_REF, rule);
    t->line = start->line;
    t->line = start->column;
    auto rref = new RuleRefElement(grammar, t, AUTO_GEN_NONE);
    rref->enclosingRuleName = ruleBlock->ruleName;
    auto end = new BlockEndElement(grammar);
    end->block = blk;
    auto alt = new Alternative(rref);
    alt->addElement(end);
    blk->addAlternative(alt);
    auto optAlt = new Alternative();
    optAlt->addElement(end);
    blk->addAlternative(optAlt);
    blk->prepareForAnalysis();
    return blk;
}

void MakeGrammar::defineRuleName(const TokenRef& r, const std::string& access, bool ruleAutoGen, const OStr& docComment) {
    if (r->type == ANTLR_TOKEN_REF) {
        if (!grammar->isLexer()) {
            tool->error("Lexical rule " + r->text + " defined outside of lexer", grammar->getFilename(), r->line, r->column);
            r->text = toLowerAscii(r->text);
        }
    } else {
        if (grammar->isLexer()) {
            tool->error("Lexical rule names must be upper case, '" + r->text + "' is not", grammar->getFilename(), r->line, r->column);
            r->text = toUpperAscii(r->text);
        }
    }

    // DefineGrammarSymbols.defineRuleName()
    std::string id = r->text;
    if (r->type == ANTLR_TOKEN_REF) {
        id = encodeLexerRuleName(id);
        if (!grammar->tokenManager->tokenDefined(r->text)) {
            int tt = grammar->tokenManager->nextTokenType();
            auto ts = new TokenSymbol(r->text);
            ts->setTokenType(tt);
            grammar->tokenManager->define(ts);
        }
    }
    RuleSymbol* rs;
    if (grammar->isDefined(id)) {
        rs = grammar->getSymbol(id);
        if (rs->isDefined())
            tool->error("redefinition of rule " + id, grammar->getFilename(), r->line, r->column);
    } else {
        rs = new RuleSymbol(id);
        grammar->define(rs);
    }
    rs->setDefined();
    rs->access = access;
    rs->comment = docComment;

    // MakeGrammar.defineRuleName()
    id = r->text;
    if (r->type == ANTLR_TOKEN_REF)
        id = encodeLexerRuleName(id);
    rs = grammar->getSymbol(id);
    auto rb = new RuleBlock(grammar, r->text, r->line, ruleAutoGen);
    rb->setDefaultErrorHandler(grammar->getDefaultErrorHandler());
    ruleBlock = rb;
    blocks.push_back(new BlockContext());
    context()->block = rb;
    rs->setBlock(rb);
    ruleEnd = new RuleEndElement(grammar);
    rb->setEndElement(ruleEnd);
    nested = 0;
}

void MakeGrammar::endAlt() {
    if (nested == 0)
        addElementToCurrentAlt(ruleEnd);
    else
        addElementToCurrentAlt(context()->blockEnd);
    context()->altNum++;
}

void MakeGrammar::endChildList() {
    auto be = new BlockEndElement(grammar);
    be->block = context()->block;
    addElementToCurrentAlt(be);
}

void MakeGrammar::endExceptionSpec() {
    if (!currentExceptionSpec)
        tool->panic("exception processing internal error -- no active exception spec");
    if (auto rb = dynamic_cast<RuleBlock*>(context()->block)) {
        rb->addExceptionSpec(currentExceptionSpec);
    } else {
        if (context()->currentAlt()->exceptionSpec)
            tool->error("Alternative already has an exception specification", grammar->getFilename(), context()->block->getLine(), context()->block->getColumn());
        else
            context()->currentAlt()->exceptionSpec = currentExceptionSpec;
    }
    currentExceptionSpec = nullptr;
}

void MakeGrammar::endGrammar() {
    if (grammarError)
        abortGrammar();
}

void MakeGrammar::endRule(const std::string&) {
    BlockContext* ctx = blocks.back();
    blocks.pop_back();
    ruleEnd->block = ctx->block;
    ruleEnd->block->prepareForAnalysis();
}

void MakeGrammar::endSubRule() {
    nested--;
    BlockContext* ctx = blocks.back();
    blocks.pop_back();
    AlternativeBlock* block = ctx->block;
    if (block->not_ && !dynamic_cast<SynPredBlock*>(block) && !dynamic_cast<ZeroOrMoreBlock*>(block) &&
        !dynamic_cast<OneOrMoreBlock*>(block)) {
        if (!analyzer->subruleCanBeInverted(block, grammar->isLexer())) {
            tool->error("This subrule cannot be inverted.  Only subrules of the form:\n"
                        "    (T1|T2|T3...) or\n"
                        "    ('c1'|'c2'|'c3'...)\n"
                        "may be inverted (ranges are also allowed).",
                        grammar->getFilename(), block->getLine(), block->getColumn());
        }
    }
    if (auto synpred = dynamic_cast<SynPredBlock*>(block)) {
        context()->block->hasASynPred = true;
        context()->currentAlt()->synPred = synpred;
        grammar->hasSyntacticPredicate = true;
        synpred->removeTrackingOfRuleRefs(grammar);
    } else {
        addElementToCurrentAlt(block);
    }
    ctx->blockEnd->block->prepareForAnalysis();
}

void MakeGrammar::endTree() {
    BlockContext* ctx = blocks.back();
    blocks.pop_back();
    addElementToCurrentAlt(ctx->block);
}

void MakeGrammar::labelElement(AlternativeElement* el, const TokenRef& label) {
    if (label) {
        for (auto altEl : ruleBlock->labeledElements) {
            OStr l = altEl->getLabel();
            if (l && *l == label->text) {
                tool->error("Label '" + label->text + "' has already been defined", grammar->getFilename(), label->line, label->column);
                return;
            }
        }
        el->setLabel(label->text);
        ruleBlock->labeledElements.push_back(el);
    }
}

void MakeGrammar::oneOrMoreSubRule() {
    if (context()->block->not_)
        tool->error("'~' cannot be applied to (...)* subrule", grammar->getFilename(), context()->block->getLine(), context()->block->getColumn());
    auto b = new OneOrMoreBlock(grammar);
    setBlock(b, context()->block);
    BlockContext* old = blocks.back();
    blocks.pop_back();
    blocks.push_back(new BlockContext());
    context()->block = b;
    context()->blockEnd = old->blockEnd;
    context()->blockEnd->block = b;
}

void MakeGrammar::optionalSubRule() {
    if (context()->block->not_)
        tool->error("'~' cannot be applied to (...)? subrule", grammar->getFilename(), context()->block->getLine(), context()->block->getColumn());
    beginAlt(false);
    endAlt();
}

void MakeGrammar::refAction(const TokenRef& action) {
    context()->block->hasAnAction = true;
    addElementToCurrentAlt(new ActionElement(grammar, action));
}

void MakeGrammar::setUserExceptions(const std::string& thr) {
    static_cast<RuleBlock*>(context()->block)->throwsSpec = thr;
}

void MakeGrammar::refArgAction(const TokenRef& action) {
    static_cast<RuleBlock*>(context()->block)->argAction = action->text;
}

void MakeGrammar::refCharLiteral(const TokenRef& lit, const TokenRef& label, bool inverted, int autoGenType, bool lastInRule) {
    if (!grammar->isLexer()) {
        tool->error("Character literal only valid in lexer", grammar->getFilename(), lit->line, lit->column);
        return;
    }
    auto lg = static_cast<LexerGrammar*>(grammar);
    auto cl = new CharLiteralElement(lg, lit, inverted, autoGenType);
    int t = cl->getType();
    if (!lg->caseSensitive && t < 128 && t >= 'A' && t <= 'Z')
        tool->warning("Character literal must be lowercase when caseSensitive=false", grammar->getFilename(), lit->line, lit->column);
    addElementToCurrentAlt(cl);
    labelElement(cl, label);
    OStr ignore = ruleBlock->getIgnoreRule();
    if (!lastInRule && ignore)
        addElementToCurrentAlt(createOptionalRuleRef(*ignore, lit));
}

void MakeGrammar::refCharRange(const TokenRef& t1, const TokenRef& t2, const TokenRef& label, int autoGenType, bool lastInRule) {
    if (!grammar->isLexer()) {
        tool->error("Character range only valid in lexer", grammar->getFilename(), t1->line, t1->column);
        return;
    }
    int rangeMin = tokenTypeForCharLiteral(t1->text);
    int rangeMax = tokenTypeForCharLiteral(t2->text);
    if (rangeMax < rangeMin) {
        tool->error("Malformed range.", grammar->getFilename(), t1->line, t1->column);
        return;
    }
    auto lg = static_cast<LexerGrammar*>(grammar);
    if (!lg->caseSensitive) {
        if (rangeMin < 128 && rangeMin >= 'A' && rangeMin <= 'Z')
            tool->warning("Character literal must be lowercase when caseSensitive=false", grammar->getFilename(), t1->line, t1->column);
        if (rangeMax < 128 && rangeMax >= 'A' && rangeMax <= 'Z')
            tool->warning("Character literal must be lowercase when caseSensitive=false", grammar->getFilename(), t2->line, t2->column);
    }
    auto cr = new CharRangeElement(lg, t1, t2, autoGenType);
    addElementToCurrentAlt(cr);
    labelElement(cr, label);
    OStr ignore = ruleBlock->getIgnoreRule();
    if (!lastInRule && ignore)
        addElementToCurrentAlt(createOptionalRuleRef(*ignore, t1));
}

void MakeGrammar::refTokensSpecElementOption(const TokenRef& tok, const TokenRef& option, const TokenRef& value) {
    TokenSymbol* ts = grammar->tokenManager->getTokenSymbol(tok->text);
    if (!ts)
        tool->panic("cannot find " + tok->text + "in tokens {...}");
    if (option->text == "AST")
        ts->ASTNodeType = value->text;
    else
        tool->error("invalid tokens {...} element option:" + option->text, grammar->getFilename(), option->line, option->column);
}

void MakeGrammar::refElementOption(const TokenRef& option, const TokenRef& value) {
    AlternativeElement* e = context()->currentElement();
    if (dynamic_cast<StringLiteralElement*>(e) || dynamic_cast<TokenRefElement*>(e) || dynamic_cast<WildcardElement*>(e))
        static_cast<GrammarAtom*>(e)->setOption(option, value);
    else
        tool->error("cannot use element option (" + option->text + ") for this kind of element", grammar->getFilename(), option->line, option->column);
}

void MakeGrammar::refExceptionHandler(const TokenRef& exTypeAndName, const TokenRef& action) {
    if (!currentExceptionSpec)
        tool->panic("exception handler processing internal error");
    currentExceptionSpec->addHandler(new ExceptionHandler(exTypeAndName, action));
}

void MakeGrammar::refInitAction(const TokenRef& action) {
    context()->block->setInitAction(action->text);
}

void MakeGrammar::refMemberAction(const TokenRef& act) {
    grammar->classMemberAction = act;
}

void MakeGrammar::refReturnAction(const TokenRef& returnAction) {
    if (grammar->isLexer()) {
        std::string name = encodeLexerRuleName(static_cast<RuleBlock*>(context()->block)->getRuleName());
        RuleSymbol* rs = grammar->getSymbol(name);
        if (rs->access == "public") {
            tool->warning("public Lexical rules cannot specify return type", grammar->getFilename(), returnAction->line, returnAction->column);
            return;
        }
    }
    static_cast<RuleBlock*>(context()->block)->returnAction = returnAction->text;
}

void MakeGrammar::refRule(const TokenRef& idAssign, const TokenRef& r, const TokenRef& label, const TokenRef& argsTok, int autoGenType) {
    if (grammar->isLexer()) {
        if (r->type != ANTLR_TOKEN_REF) {
            tool->error("Parser rule " + r->text + " referenced in lexer");
            return;
        }
        if (autoGenType == AUTO_GEN_CARET)
            tool->error("AST specification ^ not allowed in lexer", grammar->getFilename(), r->line, r->column);
    }

    // DefineGrammarSymbols.refRule()
    std::string id = r->text;
    if (r->type == ANTLR_TOKEN_REF)
        id = encodeLexerRuleName(id);
    if (!grammar->isDefined(id))
        grammar->define(new RuleSymbol(id));

    lastRuleRef = new RuleRefElement(grammar, r, autoGenType);
    if (argsTok)
        lastRuleRef->args = argsTok->text;
    if (idAssign)
        lastRuleRef->idAssign = idAssign->text;
    addElementToCurrentAlt(lastRuleRef);
    RuleSymbol* rs = grammar->getSymbol(id);
    rs->addReference(lastRuleRef);
    labelElement(lastRuleRef, label);
}

void MakeGrammar::refSemPred(const TokenRef& pred) {
    if (context()->currentAlt()->atStart()) {
        context()->currentAlt()->semPred = pred->text;
    } else {
        auto a = new ActionElement(grammar, pred);
        a->isSemPred = true;
        addElementToCurrentAlt(a);
    }
}

void MakeGrammar::refStringLiteral(const TokenRef& lit, const TokenRef& label, int autoGenType, bool lastInRule) {
    _refStringLiteral(lit);
    if (grammar->isTreeWalker() && autoGenType == AUTO_GEN_CARET)
        tool->error("^ not allowed in here for tree-walker", grammar->getFilename(), lit->line, lit->column);
    auto sl = new StringLiteralElement(grammar, lit, autoGenType);
    if (grammar->isLexer() && !static_cast<LexerGrammar*>(grammar)->caseSensitive) {
        for (std::size_t i = 1; i + 1 < lit->text.size(); i++) {
            char c = lit->text[i];
            if (c >= 'A' && c <= 'Z') {
                tool->warning("Characters of string literal must be lowercase when caseSensitive=false", grammar->getFilename(), lit->line, lit->column);
                break;
            }
        }
    }
    addElementToCurrentAlt(sl);
    labelElement(sl, label);
    OStr ignore = ruleBlock->getIgnoreRule();
    if (!lastInRule && ignore)
        addElementToCurrentAlt(createOptionalRuleRef(*ignore, lit));
}

void MakeGrammar::refToken(const TokenRef& idAssign, const TokenRef& t, const TokenRef& label, const TokenRef& argsTok, bool inverted, int autoGenType, bool lastInRule) {
    if (grammar->isLexer()) {
        if (autoGenType == AUTO_GEN_CARET)
            tool->error("AST specification ^ not allowed in lexer", grammar->getFilename(), t->line, t->column);
        if (inverted)
            tool->error("~TOKEN is not allowed in lexer", grammar->getFilename(), t->line, t->column);
        refRule(idAssign, t, label, argsTok, autoGenType);
        OStr ignore = ruleBlock->getIgnoreRule();
        if (!lastInRule && ignore)
            addElementToCurrentAlt(createOptionalRuleRef(*ignore, t));
    } else {
        if (idAssign)
            tool->error("Assignment from token reference only allowed in lexer", grammar->getFilename(), idAssign->line, idAssign->column);
        if (argsTok)
            tool->error("Token reference arguments only allowed in lexer", grammar->getFilename(), argsTok->line, argsTok->column);
        _refToken(t);
        auto te = new TokenRefElement(grammar, t, inverted, autoGenType);
        addElementToCurrentAlt(te);
        labelElement(te, label);
    }
}

void MakeGrammar::refTokenRange(const TokenRef& t1, const TokenRef& t2, const TokenRef& label, int autoGenType, bool) {
    if (grammar->isLexer()) {
        tool->error("Token range not allowed in lexer", grammar->getFilename(), t1->line, t1->column);
        return;
    }
    // DefineGrammarSymbols.refTokenRange()
    if (t1->text[0] == '"')
        _refStringLiteral(t1);
    else
        _refToken(t1);
    if (t2->text[0] == '"')
        _refStringLiteral(t2);
    else
        _refToken(t2);

    auto tr = new TokenRangeElement(grammar, t1, t2, autoGenType);
    if (tr->end < tr->begin) {
        tool->error("Malformed range.", grammar->getFilename(), t1->line, t1->column);
        return;
    }
    addElementToCurrentAlt(tr);
    labelElement(tr, label);
}

void MakeGrammar::refWildcard(const TokenRef& t, const TokenRef& label, int autoGenType) {
    auto wc = new WildcardElement(grammar, t, autoGenType);
    addElementToCurrentAlt(wc);
    labelElement(wc, label);
}

void MakeGrammar::reset() {
    grammar = nullptr;
    blocks.clear();
    lastRuleRef = nullptr;
    ruleEnd = nullptr;
    ruleBlock = nullptr;
    nested = 0;
    currentExceptionSpec = nullptr;
    grammarError = false;
}

void MakeGrammar::setBlock(AlternativeBlock* b, AlternativeBlock* src) {
    b->alternatives = src->alternatives;
    b->initAction = src->initAction;
    b->label = src->label;
    b->hasASynPred = src->hasASynPred;
    b->hasAnAction = src->hasAnAction;
    b->warnWhenFollowAmbig = src->warnWhenFollowAmbig;
    b->generateAmbigWarnings = src->generateAmbigWarnings;
    b->line = src->line;
    b->greedy = src->greedy;
    b->greedySet = src->greedySet;
}

void MakeGrammar::setRuleOption(const TokenRef& key, const TokenRef& value) {
    ruleBlock->setOption(key, value);
}

void MakeGrammar::setSubruleOption(const TokenRef& key, const TokenRef& value) {
    context()->block->setOption(key, value);
}

void MakeGrammar::synPred() {
    if (context()->block->not_)
        tool->error("'~' cannot be applied to syntactic predicate", grammar->getFilename(), context()->block->getLine(), context()->block->getColumn());
    auto b = new SynPredBlock(grammar);
    setBlock(b, context()->block);
    BlockContext* old = blocks.back();
    blocks.pop_back();
    blocks.push_back(new BlockContext());
    context()->block = b;
    context()->blockEnd = old->blockEnd;
    context()->blockEnd->block = b;
}

void MakeGrammar::zeroOrMoreSubRule() {
    if (context()->block->not_)
        tool->error("'~' cannot be applied to (...)+ subrule", grammar->getFilename(), context()->block->getLine(), context()->block->getColumn());
    auto b = new ZeroOrMoreBlock(grammar);
    setBlock(b, context()->block);
    BlockContext* old = blocks.back();
    blocks.pop_back();
    blocks.push_back(new BlockContext());
    context()->block = b;
    context()->blockEnd = old->blockEnd;
    context()->blockEnd->block = b;
}

} // namespace antlrcpp
