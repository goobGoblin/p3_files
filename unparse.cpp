#include "ast.hpp"

namespace a_lang{

/*
doIndent is declared static, which means that it can 
only be called in this file (its symbol is not exported).
*/
static void doIndent(std::ostream& out, int indent){
	for (int k = 0 ; k < indent; k++){ out << "\t"; }
}

/*
In this code, the intention is that functions are grouped 
into files by purpose, rather than by class.
If you're used to having all of the functions of a class 
defined in the same file, this style may be a bit disorienting,
though it is legal. Thus, we can have
ProgramNode::unparse, which is the unparse method of ProgramNodes
defined in the same file as DeclNode::unparse, the unparse method
of DeclNodes.
*/


void ProgramNode::unparse(std::ostream& out, int indent){
	/* Oh, hey it's a for-each loop in C++!
	   The loop iterates over each element in a collection
	   without that gross i++ nonsense. 
	 */
	for (auto global : *myGlobals){
		/* The auto keyword tells the compiler
		   to (try to) figure out what the
		   type of a variable should be from 
		   context. here, since we're iterating
		   over a list of DeclNode *s, it's 
		   pretty clear that global is of 
		   type DeclNode *.
		*/
		global->unparse(out, indent);
	}
}

void VarDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myID->unparse(out, 0);
	out << ": ";
	this->myType->unparse(out, 0);
	out << ";\n";
}


/** Type Nodes **/

void IntTypeNode::unparse(std::ostream& out, int indent){
	out << "int";
}

void BoolTypeNode::unparse(std::ostream& out, int indent){
    out << "bool";
}


/** More complex types **/

void ClassTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myID->unparse(out, 0);
}

void VoidTypeNode::unparse(std::ostream& out, int indent){
    out << "void";
}

void ImmutableTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "immutable ";
	mySub->unparse(out, 0);
}

void RefTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "& ";
	mySub->unparse(out, 0);
}


/** Expression Nodes **/

void CallExpNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myCallee->unparse(out, 0);
	out << "(";
	
	bool firstArg = true;
	for(auto arg : *myArgs){
		if (firstArg) { firstArg = false; }
		else { out << ", "; }
		arg->unparse(out, 0);
	}
	out << ")";
}

void IntLitNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << myNum;
}

void StrLitNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << myStr;
}

void TrueNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "true";
}

void FalseNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "false";
}

void EhNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "eh?";
}

// Binary Expression Nodes

void ExpNode::unparseNested(std::ostream& out){
	out << "(";
	unparse(out, 0);
	out << ")";
}

void PlusNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " + ";
	myExp2->unparseNested(out);
}

void MinusNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " - ";
	myExp2->unparseNested(out);
}

void TimesNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " * ";
	myExp2->unparseNested(out);
}

void DivideNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " / ";
	myExp2->unparseNested(out);
}

void AndNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " and ";
	myExp2->unparseNested(out);
}

void OrNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " or ";
	myExp2->unparseNested(out);
}

void EqualsNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " == ";
	myExp2->unparseNested(out);
}

void NotEqualsNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " != ";
	myExp2->unparseNested(out);
}

void LessNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " < ";
	myExp2->unparseNested(out);
}

void LessEqNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " <= ";
	myExp2->unparseNested(out);
}

void GreaterNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " > ";
	myExp2->unparseNested(out);
}

void GreaterEqNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " >= ";
	myExp2->unparseNested(out);
}

// Unary Expression Nodes

void NegNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "-";
	myExp->unparseNested(out); 
}

void NotNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "!";
	myExp->unparseNested(out); 
}


/** Statement Nodes **/

void AssignStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myDst->unparse(out, 0);
	out << " = ";
	mySrc->unparse(out, 0);
	out << ";\n";
}

void CallStmtNode::unparse(std::ostream& out, int indent){
	if (indent != -1){ doIndent(out, indent); }
	myCallExp->unparse(out, 0);
	if (indent != -1){ out << ";\n"; }
}

void ReturnStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "return";
	if (myExp != nullptr){
		out << " ";
		myExp->unparse(out, 0);
	}
	out << ";\n";
}

void MaybeStmtNode::unparse(std::ostream& out, int indent){
	if (indent != -1){ doIndent(out, indent); }
	out << "maybe ";
	myDst->unparse(out, 0);
	out << " means ";
	mySrc1->unparse(out, 0);
	out << " otherwise ";
	mySrc2->unparse(out, 0);
	if (indent != -1){ out << ";\n"; }
}

// Console statement nodes

void FromConsoleStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "fromconsole ";
	myDst->unparse(out,0);
	out << ";\n";
}

void ToConsoleStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "toconsole ";
	mySrc->unparse(out,0);
	out << ";\n";
}

// Increment and Decrement Statement Nodes

void PostDecStmtNode::unparse(std::ostream& out, int indent){
	if (indent != -1){ doIndent(out, indent); }
	this->myLoc->unparse(out,0);
	out << "--";
	if (indent != -1){ out << ";\n"; }
}

void PostIncStmtNode::unparse(std::ostream& out, int indent){
	if (indent != -1){ doIndent(out, indent); }
	
	this->myLoc->unparse(out,0);
	out << "++";

	if (indent != -1){ out << ";\n"; }
}

/* block statements */

void IfStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "if (";
	myCond->unparse(out, 0);
	out << "){\n";
	for (auto stmt : *myBody){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "}\n";
}

void IfElseStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "if (";
	myCond->unparse(out, 0);
	out << "){\n";
	for (auto stmt : *myBodyTrue){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "} else {\n";
	for (auto stmt : *myBodyFalse){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "}\n";
}

void WhileStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "while (";
	myCond->unparse(out, 0);
	out << "){\n";
	for (auto stmt : *myBody){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "}\n";
}

/** Declaration Nodes **/

void ClassDefnNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myID->unparse(out, 0);
	out << " : custom {\n";
	for(auto member : *myMembers){
		member->unparse(out, indent+1);
	}  
	out << "};\n";
}

void FormalDeclNode::unparse(std::ostream& out, int indent){
    doIndent(out, indent); 
    ID()->unparse(out, 0);
    out << " : ";
    getTypeNode()->unparse(out, 0);
}

void FnDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent); 
	myID->unparse(out, 0);
	out << " : ";
	out << "(";
	bool firstFormal = true;
	for(auto formal : *myFormals){
		if (firstFormal) { firstFormal = false; }
		else { out << ", "; }
		formal->unparse(out, 0);
	}
	out << ") -> ";
	myRetType->unparse(out, 0); 
	out << " {\n";
	for(auto stmt : *myBody){
		stmt->unparse(out, indent+1);
	}
	doIndent(out, indent);
	out << "}\n";
}

void IDNode::unparse(std::ostream& out, int indent){
	out << this->name;
}

} // End namespace a_lang
