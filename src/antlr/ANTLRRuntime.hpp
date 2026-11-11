#ifndef INC_ANTLRRuntime_hpp_
#define INC_ANTLRRuntime_hpp_

/*
 * ANTLR 2.7.7 C++ runtime, reduced to the parts used by the lexers and
 * parsers that antlrcpp generates.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include <string>
#include <string_view>
#include <algorithm>
#include <vector>
#include <map>
#include <stack>
#include <istream>
#include <iostream>
#include <exception>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <BlockPool.hpp>

#ifndef ANTLR_API
# define ANTLR_API
#endif
#ifndef CUSTOM_API
# define CUSTOM_API
#endif
#define ANTLR_USE_NAMESPACE(_x_) _x_::
#define ANTLR_BEGIN_NAMESPACE(_x_) namespace _x_ {
#define ANTLR_END_NAMESPACE }

// lookahead is called constantly from very large generated functions, where it would otherwise not be inlined
#if defined(__GNUC__) || defined(__clang__)
#define ANTLR_ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define ANTLR_ALWAYS_INLINE __forceinline
#else
#define ANTLR_ALWAYS_INLINE inline
#endif

namespace antlr {

inline std::string charName(int ch) {
	if (ch == EOF)
		return "EOF";
	std::string s;
	ch = ch & 0xFF;
	if (std::isprint(ch)) {
		s.append("'");
		s += static_cast<char>(ch);
		s.append("'");
	} else {
		static const char hex[] = "0123456789ABCDEF";
		s += "0x";
		s += hex[ch >> 4];
		s += hex[ch & 0xF];
	}
	return s;
}

/* Tokens */

class Token;

template <class T>
class TokenRefCount {
private:
	Token* ptr;

	void acquire();
	static void release(Token* p);

public:
	TokenRefCount(const Token* p = 0) : ptr(const_cast<Token*>(p)) { acquire(); }
	TokenRefCount(const TokenRefCount<T>& other) : ptr(other.ptr) { acquire(); }
	TokenRefCount(TokenRefCount<T>&& other) noexcept : ptr(other.ptr) { other.ptr = 0; }
	~TokenRefCount() { release(ptr); }
	TokenRefCount<T>& operator=(Token* other) {
		// the new token is acquired before the old is released, in case the old token holds the only reference
		Token* old = ptr;
		ptr = other;
		acquire();
		release(old);
		return *this;
	}
	TokenRefCount<T>& operator=(const TokenRefCount<T>& other) {
		if (other.ptr != ptr)
			*this = other.ptr;
		return *this;
	}
	TokenRefCount<T>& operator=(TokenRefCount<T>&& other) noexcept {
		if (this != &other) {
			Token* old = ptr;
			ptr = other.ptr;
			other.ptr = 0;
			release(old);
		}
		return *this;
	}
	operator T*() const { return static_cast<T*>(ptr); }
	T* operator->() const { return static_cast<T*>(ptr); }
	T* get() const { return static_cast<T*>(ptr); }
};

typedef TokenRefCount<Token> RefToken;

/// the token of srcML, with its category and positions, allocated from a pool of deleted tokens
class Token {
public:
	static const int MIN_USER_TYPE = 4;
	static const int NULL_TREE_LOOKAHEAD = 3;
	static const int INVALID_TYPE = 0;
	static const int EOF_TYPE = 1;
	static const int SKIP = -1;

	Token() {}
	Token(int t) : type(t) {}
	Token(int t, int cat) : type(t), category(cat) {}
	Token(int t, const std::string& txt) : type(t), text(txt) {}

	static RefToken factory();

	int getColumn() const { return column; }
	int getLine() const { return line; }
	int getEndLine() const { return endline; }
	int getEndColumn() const { return endcolumn; }
	const std::string& getText() const { return text; }
	int getType() const { return type; }
	void setColumn(int c) { column = c; }
	void setLine(int l) { line = l; }
	void setText(std::string_view s) { text = s; }
	void setType(int t) { type = t; }
	std::string toString() const {
		return "[\"" + getText() + "\",<" + std::to_string(type) + ">,line=" + std::to_string(getLine()) +
		       ",column=" + std::to_string(getColumn()) + "]";
	}

	// reuse the memory of deleted tokens, since tokens are created and deleted constantly
	static void* operator new(std::size_t size) {
		if (void* p = FreeTokens::take())
			return p;
		return ::operator new(size);
	}
	static void operator delete(void* p) {
		if (!FreeTokens::give(p))
			::operator delete(p);
	}

private:
	// number of references to the token, which is deleted with its last reference
	template <class T>
	friend class TokenRefCount;
	unsigned int refCount = 0;
	int type = INVALID_TYPE;

public:
	/** token category */
	int category = -1;

	/** start and end positions */
	int line = 0;
	int column = 0;
	int endline = 0;
	int endcolumn = 0;

	/** token text */
	std::string text;

private:
	/** maximum number of deleted tokens kept for reuse per thread */
	static constexpr std::size_t MAX_FREE_TOKENS = 4096;

	/** memory of deleted tokens, per thread */
	typedef BlockPool<Token, MAX_FREE_TOKENS> FreeTokens;

	Token(RefToken other);
	Token& operator=(const Token& other);
	Token& operator=(RefToken other);
	Token(const Token&);
};

template <class T>
inline void TokenRefCount<T>::acquire() {
	if (ptr)
		++ptr->refCount;
}

template <class T>
inline void TokenRefCount<T>::release(Token* p) {
	if (p && --p->refCount == 0)
		delete p;
}

inline RefToken nullToken;

inline RefToken Token::factory() { return RefToken(new Token); }

class TokenStream {
public:
	virtual RefToken nextToken() = 0;
	virtual ~TokenStream() {}
};

/* Reference counting */

template <class T>
class RefCount {
private:
	struct Ref {
		T* const ptr;
		unsigned int count;

		Ref(T* p) : ptr(p), count(1) {}
		~Ref() { delete ptr; }
		Ref* increment() { ++count; return this; }
		bool decrement() { return --count == 0; }

	private:
		Ref(const Ref&);
		Ref& operator=(const Ref&);
	}* ref;

public:
	explicit RefCount(T* p = 0) : ref(p ? new Ref(p) : 0) {}
	RefCount(const RefCount<T>& other) : ref(other.ref ? other.ref->increment() : 0) {}
	~RefCount() {
		if (ref && ref->decrement())
			delete ref;
	}
	RefCount<T>& operator=(const RefCount<T>& other) {
		Ref* tmp = other.ref ? other.ref->increment() : 0;
		if (ref && ref->decrement())
			delete ref;
		ref = tmp;
		return *this;
	}
	operator T*() const { return ref ? ref->ptr : 0; }
	T* operator->() const { return ref ? ref->ptr : 0; }
	T* get() const { return ref ? ref->ptr : 0; }
};

/* BitSet */

class BitSet {
private:
	std::vector<bool> storage;

public:
	BitSet(unsigned int nbits = 64) : storage(nbits, false) {}
	BitSet(const unsigned long* bits_, unsigned int nlongs) : storage(nlongs * 32) {
		for (unsigned int i = 0; i < nlongs * 32; i++)
			storage[i] = (bits_[i >> 5] & (1UL << (i & 31))) ? true : false;
	}

	void add(unsigned int el) {
		if (el >= storage.size())
			storage.resize(el + 1, false);
		storage[el] = true;
	}
	bool member(unsigned int el) const {
		if (el >= storage.size())
			return false;
		return storage[el];
	}
	std::vector<unsigned int> toArray() const {
		std::vector<unsigned int> elems;
		for (unsigned int i = 0; i < storage.size(); i++)
			if (storage[i])
				elems.push_back(i);
		return elems;
	}
};

/* Exceptions */

class ANTLRException {
public:
	ANTLRException() : text("") {}
	ANTLRException(const std::string& s) : text(s) {}
	virtual ~ANTLRException() throw() {}

	virtual std::string toString() const { return text; }
	virtual std::string getMessage() const { return text; }

private:
	std::string text;
};

class RecognitionException : public ANTLRException {
public:
	RecognitionException() : ANTLRException("parsing error"), line(-1), column(-1) {}
	RecognitionException(const std::string& s) : ANTLRException(s), line(-1), column(-1) {}
	RecognitionException(const std::string& s, const std::string& fileName_, int line_, int column_)
	    : ANTLRException(s), fileName(fileName_), line(line_), column(column_) {}
	virtual ~RecognitionException() throw() {}

	virtual std::string getFilename() const throw() { return fileName; }
	virtual int getLine() const throw() { return line; }
	virtual int getColumn() const throw() { return column; }
	virtual std::string toString() const { return getFileLineColumnString() + getMessage(); }
	virtual std::string getFileLineColumnString() const {
		std::string s;
		if (fileName.length() > 0)
			s = fileName + ":";
		if (line != -1) {
			if (fileName.length() == 0)
				s += "line ";
			s += std::to_string(line);
			if (column != -1)
				s += ":" + std::to_string(column);
			s += ":";
		}
		s += " ";
		return s;
	}

protected:
	std::string fileName;
	int line;
	int column;
};

class SemanticException : public RecognitionException {
public:
	SemanticException(const std::string& s) : RecognitionException(s) {}
	SemanticException(const std::string& s, const std::string& fileName_, int line_, int column_)
	    : RecognitionException(s, fileName_, line_, column_) {}
	~SemanticException() throw() {}
};

class CharStreamException : public ANTLRException {
public:
	CharStreamException(const std::string& s) : ANTLRException(s) {}
	~CharStreamException() throw() {}
};

class CharStreamIOException : public CharStreamException {
public:
	std::exception io;

	CharStreamIOException(std::exception& e) : CharStreamException(e.what()), io(e) {}
	~CharStreamIOException() throw() {}
};

class TokenStreamException : public ANTLRException {
public:
	TokenStreamException() : ANTLRException() {}
	TokenStreamException(const std::string& s) : ANTLRException(s) {}
	virtual ~TokenStreamException() throw() {}
};

class TokenStreamIOException : public TokenStreamException {
public:
	TokenStreamIOException() : TokenStreamException() {}
	TokenStreamIOException(const std::exception& e) : TokenStreamException(e.what()), io(e) {}
	~TokenStreamIOException() throw() {}

private:
	std::exception io;
};

class TokenStreamRecognitionException : public TokenStreamException {
public:
	TokenStreamRecognitionException(RecognitionException& re) : TokenStreamException(re.getMessage()), recog(re) {}
	virtual ~TokenStreamRecognitionException() throw() {}

	virtual std::string toString() const { return recog.getFileLineColumnString() + getMessage(); }
	virtual std::string getFilename() const throw() { return recog.getFilename(); }
	virtual int getLine() const throw() { return recog.getLine(); }
	virtual int getColumn() const throw() { return recog.getColumn(); }

private:
	RecognitionException recog;
};

class TokenStreamRetryException : public TokenStreamException {
public:
	TokenStreamRetryException() {}
	~TokenStreamRetryException() throw() {}
};

class NoViableAltException : public RecognitionException {
public:
	const RefToken token;

	NoViableAltException(RefToken t, const std::string& fileName_)
	    : RecognitionException("NoViableAlt", fileName_, t->getLine(), t->getColumn()), token(t) {}
	~NoViableAltException() throw() {}

	std::string getMessage() const {
		if (token->getType() == Token::EOF_TYPE)
			return "unexpected end of file";
		if (token->getType() == Token::NULL_TREE_LOOKAHEAD)
			return "unexpected end of tree";
		return "unexpected token: " + token->getText();
	}
};

class MismatchedTokenException : public RecognitionException {
public:
	static const int TOKEN = 1;
	static const int NOT_TOKEN = 2;
	static const int RANGE = 3;
	static const int NOT_RANGE = 4;
	static const int SET = 5;
	static const int NOT_SET = 6;

	MismatchedTokenException(const char* const* tokenNames_, const int numTokens_, RefToken token_, int expecting_, bool matchNot,
	                         const std::string& fileName_)
	    : RecognitionException("Mismatched Token", fileName_, token_->getLine(), token_->getColumn()), token(token_),
	      tokenText(token_->getText()), mismatchType(matchNot ? NOT_TOKEN : TOKEN), expecting(expecting_), upper(0),
	      tokenNames(tokenNames_), numTokens(numTokens_) {}
	MismatchedTokenException(const char* const* tokenNames_, const int numTokens_, RefToken token_, BitSet set_, bool matchNot,
	                         const std::string& fileName_)
	    : RecognitionException("Mismatched Token", fileName_, token_->getLine(), token_->getColumn()), token(token_),
	      tokenText(token_->getText()), mismatchType(matchNot ? NOT_SET : SET), expecting(0), upper(0), set(set_),
	      tokenNames(tokenNames_), numTokens(numTokens_) {}
	~MismatchedTokenException() throw() {}

	std::string getMessage() const {
		std::string s;
		switch (mismatchType) {
		case TOKEN:
			s += "expecting " + tokenName(expecting) + ", found '" + tokenText + "'";
			break;
		case NOT_TOKEN:
			s += "expecting anything but " + tokenName(expecting) + "; got it anyway";
			break;
		case SET:
		case NOT_SET: {
			s += std::string("expecting ") + (mismatchType == NOT_SET ? "NOT " : "") + "one of (";
			std::vector<unsigned int> elems = set.toArray();
			for (unsigned int i = 0; i < elems.size(); i++) {
				s += " ";
				s += tokenName(elems[i]);
			}
			s += "), found '" + tokenText + "'";
			break;
		}
		default:
			s = RecognitionException::getMessage();
			break;
		}
		return s;
	}

	const RefToken token;
	std::string tokenText;
	int mismatchType;
	int expecting;
	int upper;
	BitSet set;

private:
	const char* const* tokenNames;
	const int numTokens;

	std::string tokenName(int tokenType) const {
		if (tokenType == Token::INVALID_TYPE)
			return "<Set of tokens>";
		if (tokenType < 0 || tokenType >= numTokens)
			return "<" + std::to_string(tokenType) + ">";
		return tokenNames[tokenType];
	}
};

/* Input buffering */

template <class T>
class CircularQueue {
public:
	CircularQueue() : storage(), m_offset(0) {}

	void clear() {
		m_offset = 0;
		storage.clear();
	}
	const T& elementAt(size_t idx) const { return storage[idx + m_offset]; }
	void removeFirst() {
		if (m_offset >= OFFSET_MAX_RESIZE) {
			storage.erase(storage.begin(), storage.begin() + m_offset + 1);
			m_offset = 0;
		} else {
			++m_offset;
		}
	}
	void removeItems(size_t nb) {
		if (nb > entries())
			nb = entries();
		if (m_offset >= OFFSET_MAX_RESIZE) {
			storage.erase(storage.begin(), storage.begin() + m_offset + nb);
			m_offset = 0;
		} else {
			m_offset += nb;
		}
	}
	void append(const T& t) { storage.push_back(t); }
	template <class It>
	void append(It first, It last) { storage.insert(storage.end(), first, last); }
	size_t entries() const { return storage.size() - m_offset; }

private:
	static const size_t OFFSET_MAX_RESIZE = 5000;
	std::vector<T> storage;
	size_t m_offset;

	CircularQueue(const CircularQueue&);
	const CircularQueue& operator=(const CircularQueue&);
};

class InputBuffer {
public:
	InputBuffer() : nMarkers(0), markerOffset(0) {}
	virtual ~InputBuffer() {}

	virtual void reset() {
		nMarkers = 0;
		markerOffset = 0;
		queue.clear();
	}
	void commit() { nMarkers--; }
	void consume() {
		if (nMarkers > 0)
			++markerOffset;
		else
			queue.removeFirst();
	}
	virtual void fill(unsigned int amount) {
		while (queue.entries() < amount + markerOffset)
			queue.append(getChar());
	}
	virtual int getChar() = 0;
	int LA(unsigned int i) {
		if (queue.entries() < i + markerOffset)
			fill(i);
		return queue.elementAt(markerOffset + i - 1);
	}
	virtual unsigned int mark() {
		nMarkers++;
		return markerOffset;
	}
	virtual bool isMarked() const { return nMarkers != 0; }
	virtual void rewind(unsigned int mark) {
		markerOffset = mark;
		nMarkers--;
	}
	virtual unsigned int entries() const { return static_cast<unsigned int>(queue.entries()) - markerOffset; }

protected:
	unsigned int nMarkers;
	unsigned int markerOffset;
	CircularQueue<int> queue;

private:
	InputBuffer(const InputBuffer& other);
	InputBuffer& operator=(const InputBuffer& other);
};

class CharBuffer : public InputBuffer {
public:
	CharBuffer(std::istream& input_) : input(input_) {}
	int getChar() { return input.get(); }

protected:
	std::istream& input;

private:
	CharBuffer(const CharBuffer& other);
	CharBuffer& operator=(const CharBuffer& other);
};

class LexerInputState {
public:
	LexerInputState(InputBuffer* inbuf)
	    : column(1), line(1), tokenStartColumn(1), tokenStartLine(1), guessing(0), filename(""), input(inbuf),
	      inputResponsible(true) {}
	LexerInputState(InputBuffer& inbuf)
	    : column(1), line(1), tokenStartColumn(1), tokenStartLine(1), guessing(0), filename(""), input(&inbuf),
	      inputResponsible(false) {}
	LexerInputState(std::istream& in)
	    : column(1), line(1), tokenStartColumn(1), tokenStartLine(1), guessing(0), filename(""), input(new CharBuffer(in)),
	      inputResponsible(true) {}
	virtual ~LexerInputState() {
		if (inputResponsible)
			delete input;
	}

	virtual void reset() {
		column = 1;
		line = 1;
		tokenStartColumn = 1;
		tokenStartLine = 1;
		guessing = 0;
		input->reset();
	}
	void setPosition(int line_, int column_) {
		line = line_;
		column = column_;
	}
	InputBuffer& getInput() { return *input; }

	int column;
	int line;
	int tokenStartColumn;
	int tokenStartLine;
	int guessing;
	std::string filename;

private:
	InputBuffer* input;
	bool inputResponsible;

	LexerInputState(const LexerInputState&);
	LexerInputState& operator=(const LexerInputState&);
};

typedef RefCount<LexerInputState> LexerSharedInputState;

/* Lexer */

class CharScanner;

class MismatchedCharException : public RecognitionException {
public:
	static const int CHAR = 1;
	static const int NOT_CHAR = 2;
	static const int RANGE = 3;
	static const int NOT_RANGE = 4;
	static const int SET = 5;
	static const int NOT_SET = 6;

	MismatchedCharException(int c, int lower, int upper_, bool matchNot, CharScanner* scanner_);
	MismatchedCharException(int c, int expecting_, bool matchNot, CharScanner* scanner_);
	MismatchedCharException(int c, BitSet set_, bool matchNot, CharScanner* scanner_);
	~MismatchedCharException() throw() {}

	std::string getMessage() const {
		std::string s;
		switch (mismatchType) {
		case CHAR:
			s += "expecting '" + charName(expecting) + "', found '" + charName(foundChar) + "'";
			break;
		case NOT_CHAR:
			s += "expecting anything but '" + charName(expecting) + "'; got it anyway";
			break;
		case RANGE:
			s += "expecting token in range: '" + charName(expecting) + "'..'" + charName(upper) + "', found '" + charName(foundChar) + "'";
			break;
		case NOT_RANGE:
			s += "expecting token NOT in range: " + charName(expecting) + "'..'" + charName(upper) + "', found '" + charName(foundChar) + "'";
			break;
		case SET:
		case NOT_SET: {
			s += std::string("expecting ") + (mismatchType == NOT_SET ? "NOT " : "") + "one of (";
			std::vector<unsigned int> elems = set.toArray();
			for (unsigned int i = 0; i < elems.size(); i++) {
				s += " '";
				s += charName(elems[i]);
				s += "'";
			}
			s += "), found '" + charName(foundChar) + "'";
			break;
		}
		default:
			s += RecognitionException::getMessage();
			break;
		}
		return s;
	}

	int mismatchType;
	int foundChar;
	int expecting;
	int upper;
	BitSet set;

protected:
	CharScanner* scanner;
};

/*
 * Text of the token being lexed. The characters of a token are appended one at a time, and for a std::string each
 * append is a call into the library, since its members are extern templates, so here the append is inline.
 */
class TokenText {
public:
	static constexpr size_t npos = std::string::npos;

	TokenText() = default;
	TokenText(const TokenText&) = delete;
	TokenText& operator=(const TokenText&) = delete;
	~TokenText() { delete[] chars; }

	TokenText& operator=(std::string_view s) {
		count = 0;
		append(s);
		return *this;
	}
	void operator+=(char c) {
		if (count == capacity)
			grow(count + 1);
		chars[count++] = c;
	}
	void operator+=(std::string_view s) { append(s); }
	void append(std::string_view s) {
		if (count + s.size() > capacity)
			grow(count + s.size());
		std::memcpy(chars + count, s.data(), s.size());
		count += s.size();
	}
	void pop_back() { --count; }
	void clear() { count = 0; }

	size_t length() const { return count; }
	size_t size() const { return count; }
	bool empty() const { return count == 0; }
	const char* data() const { return chars; }
	char operator[](size_t i) const { return chars[i]; }
	std::string substr(size_t pos, size_t n = npos) const {
		return std::string(chars + pos, std::min(n, count - pos));
	}
	operator std::string_view() const { return std::string_view(chars, count); }

private:
	// out of line, since it is rare
	void grow(size_t n);

	char* chars = nullptr;
	size_t count = 0;
	size_t capacity = 0;
};

inline void TokenText::grow(size_t n) {
	size_t newCapacity = std::max(n, capacity ? capacity * 2 : size_t(64));
	char* newChars = new char[newCapacity];
	if (count)
		std::memcpy(newChars, chars, count);
	delete[] chars;
	chars = newChars;
	capacity = newCapacity;
}

class CharScannerLiteralsLess {
private:
	const CharScanner* scanner;

public:
	CharScannerLiteralsLess(const CharScanner* theScanner) : scanner(theScanner) {}
	bool operator()(const std::string& x, const std::string& y) const;
};

class CharScanner : public TokenStream {
public:
	CharScanner(InputBuffer& cb, bool case_sensitive)
	    : saveConsumedInput(true), caseSensitive(case_sensitive), literals(CharScannerLiteralsLess(this)),
	      inputState(new LexerInputState(cb)), commitToPath(false), tabsize(8) {
	}
	CharScanner(InputBuffer* cb, bool case_sensitive)
	    : saveConsumedInput(true), caseSensitive(case_sensitive), literals(CharScannerLiteralsLess(this)),
	      inputState(new LexerInputState(cb)), commitToPath(false), tabsize(8) {
	}
	CharScanner(const LexerSharedInputState& state, bool case_sensitive)
	    : saveConsumedInput(true), caseSensitive(case_sensitive), literals(CharScannerLiteralsLess(this)),
	      inputState(state), commitToPath(false), tabsize(8) {
	}
	virtual ~CharScanner() {}

	int LA(unsigned int i) {
		int c = inputState->getInput().LA(i);
		if (caseSensitive)
			return c;
		return toLower(c);
	}
	virtual void append(char c) {
		if (saveConsumedInput)
			text += c;
	}
	virtual void append(const std::string& s) {
		if (saveConsumedInput)
			text += s;
	}
	virtual void commit() { inputState->getInput().commit(); }
	virtual void recover(const RecognitionException&, const BitSet& tokenSet) {
		consume();
		consumeUntil(tokenSet);
	}
	virtual void consume() {
		if (inputState->guessing == 0) {
			// the raw character is appended, and is the same as LA(1) when case sensitive
			int c = inputState->getInput().LA(1);
			if (saveConsumedInput)
				text += static_cast<char>(c);
			if (c == '\t')
				tab();
			else
				inputState->column++;
		}
		inputState->getInput().consume();
	}
	virtual void consumeUntil(int c) {
		for (;;) {
			int la_1 = LA(1);
			if (la_1 == EOF_CHAR || la_1 == c)
				break;
			consume();
		}
	}
	virtual void consumeUntil(const BitSet& set) {
		for (;;) {
			int la_1 = LA(1);
			if (la_1 == EOF_CHAR || set.member(la_1))
				break;
			consume();
		}
	}
	virtual unsigned int mark() { return inputState->getInput().mark(); }
	virtual void rewind(unsigned int pos) { inputState->getInput().rewind(pos); }
	virtual void match(int c) {
		int la_1 = LA(1);
		if (la_1 != c)
			throw MismatchedCharException(la_1, c, false, this);
		consume();
	}
	virtual void match(const BitSet& b) {
		int la_1 = LA(1);
		if (!b.member(la_1))
			throw MismatchedCharException(la_1, b, false, this);
		consume();
	}
	virtual void match(const char* s) {
		while (*s != '\0') {
			int la_1 = LA(1), c = (*s++ & 0xFF);
			if (la_1 != c)
				throw MismatchedCharException(la_1, c, false, this);
			consume();
		}
	}
	virtual void match(const std::string& s) {
		size_t len = s.length();
		for (size_t i = 0; i < len; i++) {
			int la_1 = LA(1), c = (s[i] & 0xFF);
			if (la_1 != c)
				throw MismatchedCharException(la_1, c, false, this);
			consume();
		}
	}
	virtual void matchNot(int c) {
		int la_1 = LA(1);
		if (la_1 == c)
			throw MismatchedCharException(la_1, c, true, this);
		consume();
	}
	virtual void matchRange(int c1, int c2) {
		int la_1 = LA(1);
		if (la_1 < c1 || la_1 > c2)
			throw MismatchedCharException(la_1, c1, c2, false, this);
		consume();
	}

	/*
	 * Generated code reports failures while guessing (syntactic predicates)
	 * by setting failed and returning, instead of throwing an exception.
	 * Outside of guessing, failures throw as usual.
	 */
	bool matchOrFail(int c) {
		int la_1 = LA(1);
		if (la_1 == c) {
			consume();
			return true;
		}
		if (inputState->guessing > 0) {
			failed = true;
			return false;
		}
		throw MismatchedCharException(la_1, c, false, this);
	}
	bool matchOrFail(const BitSet& b) {
		int la_1 = LA(1);
		if (b.member(la_1)) {
			consume();
			return true;
		}
		if (inputState->guessing > 0) {
			failed = true;
			return false;
		}
		throw MismatchedCharException(la_1, b, false, this);
	}
	bool matchOrFail(const char* s) {
		while (*s != '\0') {
			if (!matchOrFail(*s++ & 0xFF))
				return false;
		}
		return true;
	}
	bool matchNotOrFail(int c) {
		int la_1 = LA(1);
		if (la_1 != c) {
			consume();
			return true;
		}
		if (inputState->guessing > 0) {
			failed = true;
			return false;
		}
		throw MismatchedCharException(la_1, c, true, this);
	}
	bool matchRangeOrFail(int c1, int c2) {
		int la_1 = LA(1);
		if (la_1 >= c1 && la_1 <= c2) {
			consume();
			return true;
		}
		if (inputState->guessing > 0) {
			failed = true;
			return false;
		}
		throw MismatchedCharException(la_1, c1, c2, false, this);
	}
	void failNoViable();
	void failSemantic(const char* predicate) {
		if (inputState->guessing > 0) {
			failed = true;
			return;
		}
		throw SemanticException(predicate);
	}
	bool failed = false;

	virtual bool getCaseSensitive() const { return caseSensitive; }
	virtual void setCaseSensitive(bool t) { caseSensitive = t; }
	virtual bool getCaseSensitiveLiterals() const = 0;
	virtual int getLine() const { return inputState->line; }
	virtual void setLine(int l) { inputState->line = l; }
	virtual int getColumn() const { return inputState->column; }
	virtual void setColumn(int c) { inputState->column = c; }
	virtual const std::string& getFilename() const { return inputState->filename; }
	virtual void setFilename(const std::string& f) { inputState->filename = f; }
	virtual bool getCommitToPath() const { return commitToPath; }
	virtual void setCommitToPath(bool commit) { commitToPath = commit; }
	virtual std::string_view getText() const { return text; }
	virtual void setText(const std::string& s) { text = s; }
	virtual void resetText() {
		text.clear();
		inputState->tokenStartColumn = inputState->column;
		inputState->tokenStartLine = inputState->line;
	}
	virtual RefToken getTokenObject() const { return _returnToken; }
	virtual void newline() {
		++inputState->line;
		inputState->column = 1;
	}
	virtual void tab() {
		int c = getColumn();
		int nc = (((c - 1) / tabsize) + 1) * tabsize + 1;
		setColumn(nc);
	}
	int setTabsize(int size) {
		int oldsize = tabsize;
		tabsize = size;
		return oldsize;
	}
	int getTabSize() const { return tabsize; }
	virtual void reportError(const RecognitionException& ex) { std::cerr << ex.toString().c_str() << std::endl; }
	virtual void reportError(const std::string& s) {
		if (getFilename() == "")
			std::cerr << "error: " << s.c_str() << std::endl;
		else
			std::cerr << getFilename().c_str() << ": error: " << s.c_str() << std::endl;
	}
	virtual void reportWarning(const std::string& s) {
		if (getFilename() == "")
			std::cerr << "warning: " << s.c_str() << std::endl;
		else
			std::cerr << getFilename().c_str() << ": warning: " << s.c_str() << std::endl;
	}
	virtual InputBuffer& getInputBuffer() { return inputState->getInput(); }
	virtual LexerSharedInputState getInputState() { return inputState; }
	virtual void setInputState(LexerSharedInputState state) { inputState = state; }
	virtual int testLiteralsTable(int ttype) const {
		std::map<std::string, int, CharScannerLiteralsLess>::const_iterator i = literals.find(std::string(text));
		if (i != literals.end())
			ttype = (*i).second;
		return ttype;
	}
	virtual int testLiteralsTable(const std::string& txt, int ttype) const {
		std::map<std::string, int, CharScannerLiteralsLess>::const_iterator i = literals.find(txt);
		if (i != literals.end())
			ttype = (*i).second;
		return ttype;
	}
	virtual int toLower(int c) const { return c == EOF_CHAR ? EOF_CHAR : std::tolower(c); }
	virtual void uponEOF() {}

	static const int EOF_CHAR = EOF;

protected:
	TokenText text;
	bool saveConsumedInput;
	bool caseSensitive;
	std::map<std::string, int, CharScannerLiteralsLess> literals;
	RefToken _returnToken;
	LexerSharedInputState inputState;
	bool commitToPath;
	int tabsize;

	virtual RefToken makeToken(int t) {
		RefToken tok(new Token(t));
		tok->column = inputState->tokenStartColumn;
		tok->line = inputState->tokenStartLine;
		return tok;
	}

private:
	CharScanner(const CharScanner& other);
	CharScanner& operator=(const CharScanner& other);
};

inline bool CharScannerLiteralsLess::operator()(const std::string& x, const std::string& y) const {
	if (scanner->getCaseSensitiveLiterals())
		return std::less<std::string>()(x, y);
	for (size_t i = 0;; ++i) {
		int c1 = i < x.size() ? std::tolower(static_cast<unsigned char>(x[i])) : 0;
		int c2 = i < y.size() ? std::tolower(static_cast<unsigned char>(y[i])) : 0;
		if (c1 != c2)
			return c1 < c2;
		if (c1 == 0)
			return false;
	}
}

inline MismatchedCharException::MismatchedCharException(int c, int lower, int upper_, bool matchNot, CharScanner* scanner_)
    : RecognitionException("Mismatched char", scanner_->getFilename(), scanner_->getLine(), scanner_->getColumn()),
      mismatchType(matchNot ? NOT_RANGE : RANGE), foundChar(c), expecting(lower), upper(upper_), scanner(scanner_) {}

inline MismatchedCharException::MismatchedCharException(int c, int expecting_, bool matchNot, CharScanner* scanner_)
    : RecognitionException("Mismatched char", scanner_->getFilename(), scanner_->getLine(), scanner_->getColumn()),
      mismatchType(matchNot ? NOT_CHAR : CHAR), foundChar(c), expecting(expecting_), upper(0), scanner(scanner_) {}

inline MismatchedCharException::MismatchedCharException(int c, BitSet set_, bool matchNot, CharScanner* scanner_)
    : RecognitionException("Mismatched char", scanner_->getFilename(), scanner_->getLine(), scanner_->getColumn()),
      mismatchType(matchNot ? NOT_SET : SET), foundChar(c), expecting(0), upper(0), set(set_), scanner(scanner_) {}

class NoViableAltForCharException : public RecognitionException {
public:
	NoViableAltForCharException(int c, CharScanner* scanner)
	    : RecognitionException("NoViableAlt", scanner->getFilename(), scanner->getLine(), scanner->getColumn()), foundChar(c) {}
	NoViableAltForCharException(int c, const std::string& fileName_, int line_, int column_)
	    : RecognitionException("NoViableAlt", fileName_, line_, column_), foundChar(c) {}
	virtual ~NoViableAltForCharException() throw() {}

	std::string getMessage() const { return std::string("unexpected char: ") + charName(foundChar); }

protected:
	int foundChar;
};

inline void CharScanner::failNoViable() {
	if (inputState->guessing > 0) {
		failed = true;
		return;
	}
	throw NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
}

/* Parser */

class TokenBuffer {
public:
	TokenBuffer(TokenStream& input_) : input(input_), nMarkers(0), markerOffset(0), numToConsume(0) {}
	virtual ~TokenBuffer() {}

	void reset() {
		nMarkers = 0;
		markerOffset = 0;
		numToConsume = 0;
		queue.clear();
		la1token = nullptr;
	}
	ANTLR_ALWAYS_INLINE int LA(unsigned int i) {
		// the token at LT(1) is cached until the position changes; its type is read each time, since it may change
		if (i == 1 && la1token)
			return la1token->getType();
		if (numToConsume > 0 || queue.entries() < i + markerOffset)
			fill(i);
		Token* token = queue.elementAt(markerOffset + i - 1).get();
		if (i == 1)
			la1token = token;
		return token->getType();
	}
	const RefToken& LT(unsigned int i) {
		if (numToConsume > 0 || queue.entries() < i + markerOffset)
			fill(i);
		return queue.elementAt(markerOffset + i - 1);
	}
	unsigned int mark() {
		syncConsume();
		nMarkers++;
		return markerOffset;
	}
	void rewind(unsigned int mark) {
		la1token = nullptr;
		syncConsume();
		markerOffset = mark;
		nMarkers--;
	}
	void consume() { numToConsume++; la1token = nullptr; }
	virtual unsigned int entries() const { return static_cast<unsigned int>(queue.entries()) - markerOffset; }

private:
	void fill(unsigned int amount) {
		syncConsume();
		while (queue.entries() < amount + markerOffset)
			queue.append(input.nextToken());
	}
	void syncConsume() {
		if (numToConsume > 0) {
			if (nMarkers > 0)
				markerOffset += numToConsume;
			else
				queue.removeItems(numToConsume);
			numToConsume = 0;
		}
	}

protected:
	TokenStream& input;
	unsigned int nMarkers;
	unsigned int markerOffset;
	unsigned int numToConsume;
	Token* la1token = nullptr;
	CircularQueue<RefToken> queue;

private:
	TokenBuffer(const TokenBuffer& other);
	const TokenBuffer& operator=(const TokenBuffer& other);
};

class ParserInputState {
public:
	ParserInputState(TokenBuffer* in) : guessing(0), filename(), input(in), inputResponsible(true) {}
	ParserInputState(TokenBuffer& in) : guessing(0), filename(""), input(&in), inputResponsible(false) {}
	virtual ~ParserInputState() {
		if (inputResponsible)
			delete input;
	}

	TokenBuffer& getInput() { return *input; }
	void reset() {
		input->reset();
		guessing = 0;
	}

	int guessing;
	std::string filename;

private:
	TokenBuffer* input;
	bool inputResponsible;

	ParserInputState(const ParserInputState&);
	ParserInputState& operator=(const ParserInputState&);
};

typedef RefCount<ParserInputState> ParserSharedInputState;

class Parser {
protected:
	Parser(TokenBuffer& input) : inputState(new ParserInputState(input)) {}
	Parser(TokenBuffer* input) : inputState(new ParserInputState(input)) {}
	Parser(const ParserSharedInputState& state) : inputState(state) {}

public:
	virtual ~Parser() {}

	virtual int LA(unsigned int i) = 0;
	virtual const RefToken& LT(unsigned int i) = 0;
	virtual std::string getFilename() const { return inputState->filename; }
	virtual void setFilename(const std::string& f) { inputState->filename = f; }
	virtual void setInputState(ParserSharedInputState state) { inputState = state; }
	virtual ParserSharedInputState getInputState() const { return inputState; }
	virtual void consume() = 0;
	virtual void consumeUntil(int tokenType) {
		while (LA(1) != Token::EOF_TYPE && LA(1) != tokenType)
			consume();
	}
	virtual void consumeUntil(const BitSet& set) {
		while (LA(1) != Token::EOF_TYPE && !set.member(LA(1)))
			consume();
	}
	virtual void match(int t) {
		if (LA(1) != t)
			throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), t, false, getFilename());
		consume();
	}
	virtual void matchNot(int t) {
		if (LA(1) == t)
			throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), t, true, getFilename());
		consume();
	}
	virtual void match(const BitSet& b) {
		if (!b.member(LA(1)))
			throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), b, false, getFilename());
		consume();
	}

	/*
	 * Generated code reports failures while guessing (syntactic predicates)
	 * by setting failed and returning, instead of throwing an exception.
	 * Outside of guessing, failures throw as usual.
	 */
	bool failed = false;

	/// flag failures outside of guessing too, for rules that handle them with the failed flag
	bool failSoftly = false;

	bool matchOrFail(int t) {
		if (LA(1) == t) {
			consume();
			return true;
		}
		if (inputState->guessing > 0 || failSoftly) {
			failed = true;
			return false;
		}
		throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), t, false, getFilename());
	}
	bool matchNotOrFail(int t) {
		if (LA(1) != t) {
			consume();
			return true;
		}
		if (inputState->guessing > 0 || failSoftly) {
			failed = true;
			return false;
		}
		throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), t, true, getFilename());
	}
	bool matchOrFail(const BitSet& b) {
		if (b.member(LA(1))) {
			consume();
			return true;
		}
		if (inputState->guessing > 0 || failSoftly) {
			failed = true;
			return false;
		}
		throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), b, false, getFilename());
	}
	void failNoViable() {
		if (inputState->guessing > 0 || failSoftly) {
			failed = true;
			return;
		}
		throw NoViableAltException(LT(1), getFilename());
	}
	void failSemantic(const char* predicate) {
		if (inputState->guessing > 0 || failSoftly) {
			failed = true;
			return;
		}
		throw SemanticException(predicate);
	}
	/// fail the current guess, or throw when not guessing
	void fail() {
		if (inputState->guessing > 0 || failSoftly) {
			failed = true;
			return;
		}
		throw RecognitionException();
	}
	/// run code that may fail a guess; false if it failed
	template <class F>
	bool attempt(F f) {
		f();
		if (failed) {
			failed = false;
			return false;
		}
		return true;
	}
	/// for rules called from user code, which expects an exception for a failed guess
	void throwIfFailed() {
		if (failed) {
			failed = false;
			throw RecognitionException("syntactic predicate failed");
		}
	}

	virtual unsigned int mark() { return inputState->getInput().mark(); }
	virtual void rewind(unsigned int pos) { inputState->getInput().rewind(pos); }
	virtual void recover(const RecognitionException&, const BitSet& tokenSet) {
		consume();
		consumeUntil(tokenSet);
	}
	virtual void reportError(const RecognitionException& ex) { std::cerr << ex.toString().c_str() << std::endl; }
	virtual void reportError(const std::string& s) {
		if (getFilename() == "")
			std::cerr << "error: " << s.c_str() << std::endl;
		else
			std::cerr << getFilename().c_str() << ": error: " << s.c_str() << std::endl;
	}
	virtual void reportWarning(const std::string& s) {
		if (getFilename() == "")
			std::cerr << "warning: " << s.c_str() << std::endl;
		else
			std::cerr << getFilename().c_str() << ": warning: " << s.c_str() << std::endl;
	}
	virtual const char* getTokenName(int num) const = 0;
	virtual const char* const* getTokenNames() const = 0;
	virtual int getNumTokens() const = 0;

protected:
	ParserSharedInputState inputState;

private:
	Parser(const Parser&);
	const Parser& operator=(const Parser&);
};

class LLkParser : public Parser {
public:
	LLkParser(const ParserSharedInputState& state, int k_) : Parser(state), k(k_) {}
	LLkParser(TokenBuffer& tokenBuf, int k_) : Parser(tokenBuf), k(k_) {}
	LLkParser(TokenStream& lexer, int k_) : Parser(new TokenBuffer(lexer)), k(k_) {}

	virtual void consume() { inputState->getInput().consume(); }
	ANTLR_ALWAYS_INLINE int LA(unsigned int i) final { return inputState->getInput().LA(i); }
	const RefToken& LT(unsigned int i) final { return inputState->getInput().LT(i); }

protected:
	int k;
};

/* Selecting between several lexers */

class TokenStreamSelector : public TokenStream {
protected:
	typedef std::map<std::string, TokenStream*> inputStreamNames_coll;
	inputStreamNames_coll inputStreamNames;
	TokenStream* input;
	typedef std::stack<TokenStream*> streamStack_coll;
	streamStack_coll streamStack;

public:
	TokenStreamSelector() : input(0) {}
	~TokenStreamSelector() {}

	void addInputStream(TokenStream* stream, const std::string& key) { inputStreamNames[key] = stream; }
	TokenStream* getCurrentStream() const { return input; }
	TokenStream* getStream(const std::string& sname) const {
		inputStreamNames_coll::const_iterator i = inputStreamNames.find(sname);
		if (i == inputStreamNames.end())
			throw std::string("TokenStream ") + sname + " not found";
		return (*i).second;
	}
	RefToken nextToken() {
		for (;;) {
			try {
				return input->nextToken();
			} catch (TokenStreamRetryException&) {
			}
		}
	}
	TokenStream* pop() {
		TokenStream* stream = streamStack.top();
		streamStack.pop();
		select(stream);
		return stream;
	}
	void push(TokenStream* stream) {
		streamStack.push(input);
		select(stream);
	}
	void push(const std::string& sname) {
		streamStack.push(input);
		select(sname);
	}
	void retry() { throw TokenStreamRetryException(); }
	void select(TokenStream* stream) { input = stream; }
	void select(const std::string& sname) { input = getStream(sname); }
};

} // namespace antlr

#endif /*INC_ANTLRRuntime_hpp_*/
