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
};

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
	VarDeclNode(const Position * p, IDNode * inID, TypeNode * inType) 
	: DeclNode(p), myID(inID), myType(inType){
		assert (myType != nullptr);
		assert (myID != nullptr);
	}
	void unparse(std::ostream& out, int indent);
private:
	IDNode * myID;
	TypeNode * myType;
};


// class ClassDefnNode : public DeclNode{
// public:
// 	ClassDefnNode(const Position * p, IDNode * inID, std::list<DeclNode *> * inMembers)
// 	: DeclNode(p), myID(inID), myMembers(inMembers){ }
// 	void unparse(std::ostream& out, int indent) override;
// 	IDNode * ID(){ return myID; }
// private:
// 	IDNode * myID;
// 	std::list<DeclNode *> * myMembers;
// };


// class FormalDeclNode : public VarDeclNode{
// public:
//     FormalDeclNode(const Position * p, IDNode * id, TypeNode * type)
//     : VarDeclNode(p, id, type, nullptr){ }
//     void unparse(std::ostream& out, int indent) override;
// };

// class FnDeclNode : public DeclNode{
// public:
// 	FnDeclNode(const Position * p,
// 	  IDNode * inID,
// 	  std::list<FormalDeclNode *> * inFormals,
// 	  TypeNode * inRetType,
// 	  std::list<StmtNode *> * inBody)
// 	: DeclNode(p), myID(inID),
// 	  myFormals(inFormals), myRetType(inRetType),
// 	  myBody(inBody){
// 	}
// 	IDNode * ID() const { return myID; }
// 	std::list<FormalDeclNode *> * getFormals() const{
// 		return myFormals;
// 	}
// 	void unparse(std::ostream& out, int indent) override;
// private:
// 	IDNode * myID;
// 	std::list<FormalDeclNode *> * myFormals;
// 	TypeNode * myRetType;
// 	std::list<StmtNode *> * myBody;
// };

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

// These are the class declarations I have added
// Bool handling
class BoolTypeNode : public TypeNode{
public:
    BoolTypeNode(const Position * p) : TypeNode(p){ }
    void unparse(std::ostream& out, int indent) override;
};



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


class IntLitNode : public ExpNode{
public:
	IntLitNode(const Position * p, const int numIn)
	: ExpNode(p), myNum(numIn){ }
	virtual void unparseNested(std::ostream& out) {
		unparse(out, 0);
	}
	void unparse(std::ostream& out, int indent) override;
private:
	const int myNum;
};

// Void handling
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


// class RefTypeNode : public TypeNode{
// public:
// 	RefTypeNode(const Position * p, TypeNode * inSub)
// 	: TypeNode(p), mySub(inSub){}
// 	void unparse(std::ostream& out, int indent) override;
// private:
// 	TypeNode * mySub;
// };


} //End namespace a_lang

#endif
