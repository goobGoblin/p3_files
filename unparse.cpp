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


// void ClassDefnNode::unparse(std::ostream& out, int indent){
// 	doIndent(out, indent);
// 	myID->unparse(out, 0);
// 	out << " : custom {\n";
// 	for(auto member : *myMembers){
// 		member->unparse(out, indent+1);
// 	}  
// 	out << "};\n";
// }



// void FormalDeclNode::unparse(std::ostream& out, int indent){
//     doIndent(out, indent); 
//     ID()->unparse(out, 0);
//     out << " : ";
//     getTypeNode()->unparse(out, 0);
// }

// void FnDeclNode::unparse(std::ostream& out, int indent){
// 	doIndent(out, indent); 
// 	myID->unparse(out, 0);
// 	out << " : ";
// 	out << "(";
// 	bool firstFormal = true;
// 	for(auto formal : *myFormals){
// 		if (firstFormal) { firstFormal = false; }
// 		else { out << ", "; }
// 		formal->unparse(out, 0);
// 	}
// 	out << ") -> ";
// 	myRetType->unparse(out, 0); 
// 	out << " {\n";
// 	for(auto stmt : *myBody){
// 		stmt->unparse(out, indent+1);
// 	}
// 	doIndent(out, indent);
// 	out << "}\n";
// }

void IDNode::unparse(std::ostream& out, int indent){
	out << this->name;
}

void IntTypeNode::unparse(std::ostream& out, int indent){
	out << "int";
}

// New definitions

// Bool
void BoolTypeNode::unparse(std::ostream& out, int indent){
    out << "bool";
}


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


void IntLitNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << myNum;
}

// Void def
void VoidTypeNode::unparse(std::ostream& out, int indent){
    out << "void";
}


void ImmutableTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "immutable ";
	mySub->unparse(out, 0);
}



// void RefTypeNode::unparse(std::ostream& out, int indent){
// 	doIndent(out, indent);
// 	out << "& ";
// 	mySub->unparse(out, 0);
// }


} // End namespace a_lang
