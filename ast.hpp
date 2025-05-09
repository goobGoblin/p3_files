#ifndef A_LANG_AST_HPP
#define A_LANG_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"
#include <cassert>


/* You'll probably want to add a bunch of ASTNode subclasses */

namespace a_lang{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/
class DeclNode;
class TypeNode;
class StmtNode;
class IDNode;

/** 
* \class ASTNode
* Base class for all other AST Node types
**/
class ASTNode{
public:
	ASTNode(const Position * p) : myPos(p){ }
	virtual void unparse(std::ostream& out, int indent) = 0;
	const Position * pos() { return myPos; }
	std::string posStr() { return pos()->span(); }
protected:
	const Position * myPos = nullptr;
};

/** 
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn) ;
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<DeclNode * > * myGlobals;
};

class StmtNode : public ASTNode{
public:
	StmtNode(const Position * p) : ASTNode(p){ }
	void unparse(std::ostream& out, int indent) override = 0;
};


/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to 
* declare a struct, function, variable, etc).  This base class will 
**/
class DeclNode : public StmtNode{
public:
	DeclNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

/**  \class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
protected:
	ExpNode(const Position * p) : ASTNode(p){ }
public:
	virtual void unparseNested(std::ostream& out);
}; // Added a virtual unparseNested to deal with expressions better

/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in 
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(const Position * p) : ASTNode(p){
	}
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

/** A memory location. LocNodes subclass ExpNode
 * because they can be used as part of an expression. 
**/
class LocNode : public ExpNode{
public:
	LocNode(const Position * p)
	: ExpNode(p) {}
	void unparse(std::ostream& out, int indent) = 0;
};

/** An identifier. Note that IDNodes subclass
 * LocNode because they are a type of memory location. 
**/
class IDNode : public LocNode{
public:
	IDNode(const Position * p, std::string nameIn) 
	: LocNode(p), name(nameIn){ }
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string name;
};

 
/** A variable declaration.
**/
class VarDeclNode : public DeclNode{
public:
	VarDeclNode(const Position * p, IDNode * inID,
	TypeNode * inType, ExpNode * inInit)
	: DeclNode(p), myID(inID), myType(inType), myInit(inInit){ }
	void unparse(std::ostream& out, int indent) override;
	IDNode * ID(){ return myID; }
	TypeNode * getTypeNode() const{ return myType; }
private:
	IDNode * myID;
	TypeNode * myType;
	ExpNode * myInit;
};

/** Type Nodes **/

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class BoolTypeNode : public TypeNode{
public:
    BoolTypeNode(const Position * p) : TypeNode(p){ }
    void unparse(std::ostream& out, int indent) override;
};

/* More complex types */

class ClassTypeNode : public TypeNode{
public:
	ClassTypeNode(const Position * p, IDNode * inID)
	: TypeNode(p), myID(inID){}
	void unparse(std::ostream& out, int indent) override;
private:
	IDNode * myID;
};

class VoidTypeNode : public TypeNode{
public:
    VoidTypeNode(const Position * p) : TypeNode(p){ }
    void unparse(std::ostream& out, int indent) override;
};

class ImmutableTypeNode : public TypeNode{
public:
	ImmutableTypeNode(const Position * p, TypeNode * inSub)
	: TypeNode(p), mySub(inSub){}
	void unparse(std::ostream& out, int indent) override;
private:
	TypeNode * mySub;
};

class RefTypeNode : public TypeNode{
public:
	RefTypeNode(const Position * p, TypeNode * inSub)
	: TypeNode(p), mySub(inSub){}
	void unparse(std::ostream& out, int indent) override;
private:
	TypeNode * mySub;
};


/** Expression Nodes **/

class CallExpNode : public ExpNode{
public:
	CallExpNode(const Position * p, LocNode * inCallee,
	  std::list<ExpNode *> * inArgs)
	: ExpNode(p), myCallee(inCallee), myArgs(inArgs){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LocNode * myCallee;
	std::list<ExpNode *> * myArgs;
};

class IntLitNode : public ExpNode{
public:
	IntLitNode(const Position * p, const int numIn)
	: ExpNode(p), myNum(numIn){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
private:
	const int myNum;
};

class StrLitNode : public ExpNode{
public:
	StrLitNode(const Position * p, const std::string strIn)
	: ExpNode(p), myStr(strIn){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
private:
	 const std::string myStr;
};

class TrueNode : public ExpNode{
public:
	TrueNode(const Position * p): ExpNode(p){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
};

class FalseNode : public ExpNode{
public:
	FalseNode(const Position * p): ExpNode(p){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
};

class EhNode : public ExpNode{
public:
	EhNode(const Position * p): ExpNode(p){ }
	virtual void unparseNested(std::ostream& out) override{
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
};

// Binary Expression Nodes

class BinaryExpNode : public ExpNode{
public:
	BinaryExpNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: ExpNode(p), myExp1(lhs), myExp2(rhs) { }
protected:
	ExpNode * myExp1;
	ExpNode * myExp2;
};

class PlusNode : public BinaryExpNode{
public:
	PlusNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class MinusNode : public BinaryExpNode{
public:
	MinusNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class TimesNode : public BinaryExpNode{
public:
	TimesNode(const Position * p, ExpNode * e1In, ExpNode * e2In)
	: BinaryExpNode(p, e1In, e2In){ }
	void unparse(std::ostream& out, int indent) override;
};

class DivideNode : public BinaryExpNode{
public:
	DivideNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class AndNode : public BinaryExpNode{
public:
	AndNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class OrNode : public BinaryExpNode{
public:
	OrNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class EqualsNode : public BinaryExpNode{
public:
	EqualsNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class NotEqualsNode : public BinaryExpNode{
public:
	NotEqualsNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class LessNode : public BinaryExpNode{
public:
	LessNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class LessEqNode : public BinaryExpNode{
public:
	LessEqNode(const Position * pos, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(pos, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class GreaterNode : public BinaryExpNode{
public:
	GreaterNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

class GreaterEqNode : public BinaryExpNode{
public:
	GreaterEqNode(const Position * p, ExpNode * e1, ExpNode * e2)
	: BinaryExpNode(p, e1, e2){ }
	void unparse(std::ostream& out, int indent) override;
};

// Unary Expression Nodes

class UnaryExpNode : public ExpNode {
public:
	UnaryExpNode(const Position * p, ExpNode * expIn)
	: ExpNode(p){
		this->myExp = expIn;
	}
	virtual void unparse(std::ostream& out, int indent) override = 0;
protected:
	ExpNode * myExp;
};

class NegNode : public UnaryExpNode{
public:
	NegNode(const Position * p, ExpNode * exp)
	: UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent) override;
};

class NotNode : public UnaryExpNode{
public:
	NotNode(const Position * p, ExpNode * exp)
	: UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent) override;
};

/** Statement Nodes **/

class AssignStmtNode : public StmtNode{
public:
	AssignStmtNode(const Position * p, LocNode * inDst, ExpNode * inSrc)
	: StmtNode(p), myDst(inDst), mySrc(inSrc){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LocNode * myDst;
	ExpNode * mySrc;
};

class CallStmtNode : public StmtNode{
public:
	CallStmtNode(const Position * p, CallExpNode * expIn)
	: StmtNode(p), myCallExp(expIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	CallExpNode * myCallExp;
};

class ReturnStmtNode : public StmtNode{
public:
	ReturnStmtNode(const Position * p, ExpNode * exp)
	: StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override;
private:
	ExpNode * myExp;
};

class MaybeStmtNode : public StmtNode{
public:
	MaybeStmtNode(const Position * p, LocNode * inDst, ExpNode * inSrc1, ExpNode * inSrc2)
	: StmtNode(p), myDst(inDst), mySrc1(inSrc1), mySrc2(inSrc2){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LocNode * myDst;
	ExpNode * mySrc1;
	ExpNode * mySrc2;
};

// Console statement nodes

class FromConsoleStmtNode : public StmtNode{
public:
	FromConsoleStmtNode(const Position * p, LocNode * inDst)
	: StmtNode(p), myDst(inDst){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LocNode * myDst;
};

class ToConsoleStmtNode : public StmtNode{
public:
	ToConsoleStmtNode(const Position * p, ExpNode * inSrc)
	: StmtNode(p), mySrc(inSrc){ }
	void unparse(std::ostream& out, int indent) override;
private:
	ExpNode * mySrc;
};

// Increment and Decrement Statement Nodes

class PostDecStmtNode : public StmtNode{
public:
	PostDecStmtNode(const Position * p, LocNode * inLoc)
	: StmtNode(p), myLoc(inLoc){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LocNode * myLoc;
};

class PostIncStmtNode : public StmtNode{
public:
	PostIncStmtNode(const Position * p, LocNode * inLoc)
	: StmtNode(p), myLoc(inLoc){ }
	void unparse(std::ostream& out, int indent) override;
private:
	LocNode * myLoc;
};

/* block statements */

class IfStmtNode : public StmtNode{
public:
	IfStmtNode(const Position * p, ExpNode * condIn,
	  std::list<StmtNode *> * bodyIn)
	: StmtNode(p), myCond(condIn), myBody(bodyIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	ExpNode * myCond;
	std::list<StmtNode *> * myBody;
};

class IfElseStmtNode : public StmtNode{
public:
	IfElseStmtNode(const Position * p, ExpNode * condIn,
	  std::list<StmtNode *> * bodyTrueIn,
	  std::list<StmtNode *> * bodyFalseIn)
	: StmtNode(p), myCond(condIn),
	  myBodyTrue(bodyTrueIn), myBodyFalse(bodyFalseIn) { }
	void unparse(std::ostream& out, int indent) override;
private:
	ExpNode * myCond;
	std::list<StmtNode *> * myBodyTrue;
	std::list<StmtNode *> * myBodyFalse;
};

class WhileStmtNode : public StmtNode{
public:
	WhileStmtNode(const Position * p, ExpNode * condIn,
	  std::list<StmtNode *> * bodyIn)
	: StmtNode(p), myCond(condIn), myBody(bodyIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	ExpNode * myCond;
	std::list<StmtNode *> * myBody;
};


/** Declaration Nodes **/

class ClassDefnNode : public DeclNode{
public:
	ClassDefnNode(const Position * p, IDNode * inID, std::list<DeclNode *> * inMembers)
	: DeclNode(p), myID(inID), myMembers(inMembers){ }
	void unparse(std::ostream& out, int indent) override;
	IDNode * ID(){ return myID; }
private:
	IDNode * myID;
	std::list<DeclNode *> * myMembers;
};

class FormalDeclNode : public VarDeclNode{
public:
    FormalDeclNode(const Position * p, IDNode * id, TypeNode * type)
    : VarDeclNode(p, id, type, nullptr){ }
    void unparse(std::ostream& out, int indent) override;
};

class FnDeclNode : public DeclNode{
public:
	FnDeclNode(const Position * p,
	  IDNode * inID,
	  std::list<FormalDeclNode *> * inFormals,
	  TypeNode * inRetType,
	  std::list<StmtNode *> * inBody)
	: DeclNode(p), myID(inID),
	  myFormals(inFormals), myRetType(inRetType),
	  myBody(inBody){
	}
	IDNode * ID() const { return myID; }
	std::list<FormalDeclNode *> * getFormals() const{
		return myFormals;
	}
	void unparse(std::ostream& out, int indent) override;
private:
	IDNode * myID;
	std::list<FormalDeclNode *> * myFormals;
	TypeNode * myRetType;
	std::list<StmtNode *> * myBody;
};

} //End namespace a_lang

#endif
