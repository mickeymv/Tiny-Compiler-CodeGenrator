/****************************************************************
              Copyright (C) 1986 by Manuel E. Bermudez
              Translated to C - 1991
*****************************************************************/

#include <stdio.h>
#include <header/Open_File.h>
#include <header/CommandLine.h>
#include <header/Table.h>
#include <header/Text.h>
#include <header/Error.h>
#include <header/String_Input.h> 
#include <header/Tree.h>
#include <header/Dcln.h>
#include <header/Constrainer.h>

#define ProgramNode    1
#define TypesNode      2
#define TypeNode       3
#define DclnsNode      4
#define VarNode        5
#define IntegerTNode   6
#define BooleanTNode   7
#define BlockNode      8
#define AssignNode     9
#define OutputNode     10
#define IfNode         11
#define WhileNode      12
#define NullNode       13
#define LENode         14
#define PlusNode       15
#define MinusNode      16
#define ReadNode       17
#define IntegerNode    18
#define IdentifierNode 19
#define ExponentiationNode 20
#define NotNode 21
#define OrNode 22
#define MultNode 23
#define DivNode 24
#define AndNode 25
#define ModNode 26
#define EqNode 27
#define NotEqNode 28
#define GTENode 29
#define LTNode 30
#define GTNode 31
#define TrueNode 32
#define FalseNode 33
#define EOFNode 34
#define RepeatNode 35
#define SwapNode 36
#define LOOP_CTXT 37
#define LoopNode 38
#define ExitNode 39
#define ForUptoNode 40
#define FOR_CTXT 41
#define ForDowntoNode 42
#define CaseNode 43
#define CaseClauseNode 44
#define OtherwiseNode 45
#define LitNode 46
#define CharTNode 47
#define CharNode 48
#define ConstsNode 49
#define ConstNode 50
#define StringNode 51
#define SuccNode 52
#define PredNode 53
#define OrdNode 54
#define ChrNode 55
#define CaseRangeNode 56

#define NumberOfNodes  56

#define IdInDeclaration  0
#define IdInUse  1

typedef TreeNode UserType;

/****************************************************************
 Add new node names to the end of the array, keeping in strict
  order with the define statements above, then adjust the i loop
  control variable in InitializeConstrainer().
*****************************************************************/
char *node[] = { "program", "types", "type", "dclns",
                 "var", "integer", "boolean", "block",
                 "assign", "output", "if", "while", 
                 "<null>", "<=", "+", "-", "read",
                 "<integer>", "<identifier>", "**",
				 "not", "or", "*", "/", "and", "mod",
				 "=", "<>", ">=", "<", ">", "true", "false",
				"eof", "repeat", "swap", "<loop_ctxt>", "loop",
				"exit", "upto", "<for_ctxt>", "downto", "case",
				"case_clause", "otherwise", "lit", "char",
				"<char>", "consts", "const", "<string>", "succ",
				"pred", "ord", "chr", ".."
                };


UserType TypeInteger, TypeBoolean, TypeChar, TypeString;
boolean TraceSpecified;
FILE *TraceFile;
char *TraceFileName;
int NumberTreesRead, index;


void Constrain(void)    
{
   int i;
   InitializeDeclarationTable();
   Tree_File = Open_File("_TREE", "r"); 
   NumberTreesRead = Read_Trees();
   fclose (Tree_File);                     

   AddIntrinsics();

#if 0
   printf("CURRENT TREE\n");
   for (i=1;i<=SizeOf(Tree);i++)
      printf("%2d: %d\n",i,Element(Tree,i));
#endif

   ProcessNode(RootOfTree(1)); 

    
   Tree_File = fopen("_TREE", "w");  
   Write_Trees();
   fclose (Tree_File);                   

   if (TraceSpecified)
      fclose(TraceFile);    
}


void InitializeConstrainer (int argc, char *argv[])
{          
   int i, j;

   InitializeTextModule();
   InitializeTreeModule();

   for (i=0, j=1; i<NumberOfNodes; i++, j++)
      String_Array_To_String_Constant (node[i], j); 
 
   index = System_Flag ("-trace", argc, argv);
 
   if (index)                                       
   {
      TraceSpecified = true; 
      TraceFileName = System_Argument ("-trace", "_TRACE", argc, argv);
      TraceFile = Open_File(TraceFileName, "w");
   }
   else
      TraceSpecified = false;          
}                        

int ModeOf(TreeNode IdNode, int IdUsage) {
	/*
	printf("\nIn modeof with id %d and usage %d\n", IdNode, IdUsage);
	*/
	/*printf("\nIn ModeOf with IdNode:%s\n",NodeName(Child(IdNode,1)));*/
	if (IdUsage == IdInDeclaration) { /*the <id> node is used in 'declaration'*/
	/*printf("\nMode of Id of %s is %d\n", NodeName(Child(IdNode,1)),Decoration(Child(IdNode,1)));*/
		return NodeName(Decoration(Child(IdNode,1)));
	} else if (IdUsage == IdInUse) { /*the <id> node is used in 'usage'*/
	
	    TreeNode Decl = Lookup(NodeName(Child(IdNode,1)),IdNode);
		/*printf("\nMode of Id of %s is %d\n", NodeName(Child(IdNode,1)),Decoration(Child(IdNode,1)));*/
	    return NodeName(Decoration(Child(Decl,1)));
	}
}

void AddIntrinsics (void)
{
   TreeNode TempTree, T;

   AddTree (TypesNode, RootOfTree(1), 2); /*Add types node under root (program) */

   TempTree = Child (RootOfTree(1), 2); /*Get the types node.*/
   AddTree (TypeNode, TempTree, 1);   /*Add typeNode (integer) to types*/

   TempTree = Child (RootOfTree(1), 2); /*Get the types node.*/
   AddTree (TypeNode, TempTree, 1);		/*Add typeNode (boolean) to types*/
   
   TempTree = Child (RootOfTree(1), 2); /*Get the types node.*/
   AddTree (TypeNode, TempTree, 1);		/*Add typeNode (char) to types*/
   
   TempTree = Child (Child (RootOfTree(1), 2), 1); /*Get first type node.*/
   AddTree (IdentifierNode, TempTree, 1);	/*Add identifier under first type node.*/
   TempTree = Child (Child (Child (RootOfTree(1), 2), 1),1);
   AddTree (IntegerTNode, TempTree, 1);	/*Add integer under first type's id node.*/
   
   TempTree = Child (Child (RootOfTree(1), 2), 3); /*Get third type node.*/
   AddTree (IdentifierNode, TempTree, 1);	/*Add identifier under third type node.*/
   TempTree = Child (Child (Child (RootOfTree(1), 2), 3),1);
   AddTree (CharTNode, TempTree, 1);	/*Add char under third type's id node.*/
 
   TempTree = Child (Child (RootOfTree(1), 2), 2);	/*Get second type node.*/
   AddTree (IdentifierNode, TempTree, 1);	/*Add identifier under second type node.*/
   TempTree = Child (Child (RootOfTree(1), 2), 2);	/*Get second type node.*/
   AddTree (LitNode, TempTree, 2);	/*Add lit under second type node.*/
   
   TempTree = Child (Child (Child (RootOfTree(1), 2), 2),1);
   AddTree (BooleanTNode, TempTree, 1); /*Add boolean under second type's id node.*/
   TempTree = Child (Child (Child (RootOfTree(1), 2), 2),2); /*Get lit node under second type (boolean's sibling)*/
   AddTree (IdentifierNode, TempTree, 1);	/*Add identifier under lit node.*/
   TempTree = Child (Child (Child (RootOfTree(1), 2), 2),2); /*Get lit node under second type (boolean's sibling)*/
   AddTree (IdentifierNode, TempTree, 2);	/*Add identifier under lit node.*/
   TempTree = Child (Child (Child (Child (RootOfTree(1), 2), 2), 2), 1);
   AddTree (FalseNode, TempTree, 1);
   TempTree = Child (Child (Child (Child (RootOfTree(1), 2), 2), 2), 2);
   AddTree (TrueNode, TempTree, 1);
   

T = Child (RootOfTree(1), 2); /*Get the types node.*/
   TypeInteger = Child(T,1);
 TypeBoolean = Child(T,2);
 TypeChar = Child(T,3);
}



void ErrorHeader (TreeNode T)
{ 
   printf ("<<< CONSTRAINER ERROR >>> AT ");
   Write_String (stdout,SourceLocation(T));
   printf (" : ");
   printf ("\n");
}


int NKids (TreeNode T)
{
   return (Rank(T));
}



UserType Expression (TreeNode T)
{
   UserType Type1, Type2;
   TreeNode Declaration, Temp1, Temp2;
   int NodeCount;

   if (TraceSpecified)
   {
      fprintf (TraceFile, "<<< CONSTRAINER >>> : Expn Processor Node ");
      Write_String (TraceFile, NodeName(T));
      fprintf (TraceFile, "\n");
   }
     
   switch (NodeName(T))
   {
      case LENode :    
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2 || Type2 != TypeInteger)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '<=' MUST BE OF TYPE INTEGER!\n");
            printf ("\n");
         }
         return (TypeBoolean);
		 
         case GTENode :    
            Type1 = Expression (Child(T,1));
            Type2 = Expression (Child(T,2));
			/*TODO: if type is not integer, coerce into int*/
			/*
            if (Type1 != Type2 || Type2 != TypeInteger)
            {
               ErrorHeader(Child(T,1));
               printf ("ARGUMENTS OF '>=' MUST BE OF TYPE INTEGER!\n");
               printf ("\n");
            }
			*/
            return (TypeBoolean);
			
	        case LTNode :    
	           Type1 = Expression (Child(T,1));
	           Type2 = Expression (Child(T,2));

	           if (Type1 != Type2 || Type2 != TypeInteger)
	           {
	              ErrorHeader(Child(T,1));
	              printf ("ARGUMENTS OF '<' MUST BE OF TYPE INTEGER!\n");
	              printf ("\n");
	           }
	           return (TypeBoolean);
			   
			   
		       case GTNode :    
		          Type1 = Expression (Child(T,1));
		          Type2 = Expression (Child(T,2));

		          if (Type1 != Type2)
		          {
		             ErrorHeader(Child(T,1));
		             printf ("ARGUMENTS OF '>' MUST BE OF same type!\n");
		             printf ("\n");
		          }
		          return (TypeBoolean);
		 
         case EqNode :    
            Type1 = Expression (Child(T,1));
            Type2 = Expression (Child(T,2));

            if (Type1 != Type2)
            {
               ErrorHeader(Child(T,1));
               printf ("ARGUMENTS OF '=' MUST BE OF SAME TYPE!\n");
               printf ("\n");
            }
            return (TypeBoolean);
			
            case NotEqNode :    
               Type1 = Expression (Child(T,1));
               Type2 = Expression (Child(T,2));

               if (Type1 != Type2)
               {
                  ErrorHeader(Child(T,1));
                  printf ("ARGUMENTS OF '<>' MUST BE OF SAME TYPE!\n");
                  printf ("\n");
               }
               return (TypeBoolean);			 
		 
         case MultNode :    
            Type1 = Expression (Child(T,1));
            Type2 = Expression (Child(T,2));

            if (Type1 != Type2 || Type2 != TypeInteger)
            {
               ErrorHeader(Child(T,1));
               printf ("ARGUMENTS OF '*' MUST BE OF TYPE INTEGER!\n");
               printf ("\n");
            }
            return (TypeInteger);	
			
            case ModNode :    
               Type1 = Expression (Child(T,1));
               Type2 = Expression (Child(T,2));

               if (Type1 != Type2 || Type2 != TypeInteger)
               {
                  ErrorHeader(Child(T,1));
                  printf ("ARGUMENTS OF 'mod' MUST BE OF TYPE INTEGER!\n");
                  printf ("\n");
               }
               return (TypeInteger);
			   
			   
               case DivNode :    
                  Type1 = Expression (Child(T,1));
                  Type2 = Expression (Child(T,2));

                  if (Type1 != Type2 || Type2 != TypeInteger)
                  {
                     ErrorHeader(Child(T,1));
                     printf ("ARGUMENTS OF '/' MUST BE OF TYPE INTEGER!\n");
                     printf ("\n");
                  }
                  return (TypeInteger);
			   
               case AndNode :    
                  Type1 = Expression (Child(T,1));
                  Type2 = Expression (Child(T,2));

                  if (Type1 != Type2 || Type2 != TypeBoolean)
                  {
                     ErrorHeader(Child(T,1));
                     printf ("ARGUMENTS OF 'and' MUST BE OF TYPE BOOLEAN!\n");
                     printf ("\n");
                  }
                  return (TypeBoolean);	   	
		 
         case OrNode :    
            Type1 = Expression (Child(T,1));
			Type2 = Expression (Child(T,2));

            if (Type1 != TypeBoolean || Type2 != TypeBoolean)
            {
               ErrorHeader(Child(T,1));
               printf ("ARGUMENTS OF 'or' MUST BE OF TYPE BOOLEAN!\n");
               printf ("\n");
            }
            return (TypeBoolean);	 


      case PlusNode :
      Type1 = Expression (Child(T,1));

      if (Rank(T) == 2)
         Type2 = Expression (Child(T,2));
      else  
         Type2 = TypeInteger;

      if (Type1 == TypeChar || Type2 == TypeChar)
      {
         ErrorHeader(Child(T,1));
         printf ("ARGUMENTS OF '+' ");
         printf ("Cannot be of TYPE char!\n");
         printf ("\n");
      }
      return (TypeInteger);
	  	
      case MinusNode : 
         Type1 = Expression (Child(T,1));

         if (Rank(T) == 2)
            Type2 = Expression (Child(T,2));
         else  
            Type2 = TypeInteger;

         if (Type1 != TypeInteger || Type2 != TypeInteger)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '-' ");
            printf ("MUST BE OF TYPE INTEGER!\n");
            printf ("\n");
         }
         return (TypeInteger);
		 
         case NotNode : 
            Type1 = Expression (Child(T,1));

            if (Type1 != TypeBoolean)
            {
               ErrorHeader(Child(T,1));
               printf ("ARGUMENT OF 'not' ");
               printf ("MUST BE of TYPE BOOLEAN\n");
               printf ("\n");
            }
            return (TypeBoolean);
		 
		 
         case ExponentiationNode : 
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2 || Type2 != TypeInteger)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '**' MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeInteger);

         case EOFNode :
            return (TypeBoolean);

      case IntegerNode : 
	  	Decorate (T,TypeInteger);
         return (TypeInteger);
		 
      case CharNode : 
	  Decorate(T, TypeChar);
            return (TypeChar);	 
		 
	
         case TrueNode : 
            return (TypeBoolean);
			
         case FalseNode : 
            return (TypeBoolean);		 


      case IdentifierNode :
         Declaration = Lookup (NodeName(Child(T,1)), T);
         if (Declaration != NullDeclaration)
         {
            Decorate (T, Declaration);
            return (Decoration(Declaration));
         }

			 
	  case SuccNode:
	  case PredNode:
	  	Type1 = Expression(Child(T,1));
		Decorate(T,Type1);
	  	return Type1;
	  
	  case OrdNode:
	    Expression(Child(T,1));
		Decorate(T,TypeInteger);
	  	return (TypeInteger);
	  
	  case ChrNode:
	   Type1 = Expression(Child(T,1));
       if (Type1 != TypeInteger)
       {
          ErrorHeader(Child(T,1));
          printf ("ARGUMENTS OF chr MUST BE TYPE INTEGER\n");
          printf ("\n");
       }
	   Decorate(T,TypeChar);
	   return TypeChar;
	   
	   case CaseRangeNode:
	   	Type1 = Expression(Child(T,1));
		Type2 = Expression(Child(T,2));
		/*
		printf ("\n\nIn case range node, types are %d and %d\n\n", Type1, Type2);
        */
		if (Type1 != Type2)
        {
           ErrorHeader(T);
           printf ("Range literals must be of same type!\n");
           printf ("\n");
        }
	   	return Type1;
	   
	   


      default :
         ErrorHeader(T);
         printf ( "UNKNOWN NODE NAME ");
         Write_String (stdout,NodeName(T));
         printf ("\n");

   }  /* end switch */
}  /* end Expression */




void ProcessNode (TreeNode T) 
{
   int Kid, N;
   String Name1, Name2, Mode, VarTypeName;
   TreeNode Decl, Type, Type1, Type2, Type3, Temp, constNode, idFirstChildNode, idSecondChildNode, idNameFirstGrandChildNode, idNodeNameFirstGrandChildNode, idNameSecondGrandChildNode, idNodeNameSecondGrandChild, intNode, charNode;
   TreeNode ConstDeclareIdNode, ConstDeclareNameNode, ConstValueIdNode, ConstValueName;
   TreeNode VarTypeIdNode, VarTypeNameNode, VarTypeNode;
   TreeNode LeftHandId, RightHandId;
   if (TraceSpecified)
   {
      fprintf (TraceFile,
               "<<< CONSTRAINER >>> : Stmt Processor Node ");
      Write_String (TraceFile, NodeName(T));
      fprintf (TraceFile, "\n");
   }

   switch (NodeName(T))
   {
      case ProgramNode : 
         OpenScope();
		 DTEnter(LOOP_CTXT,T,T);
		 DTEnter(FOR_CTXT,T,T);
         Name1 = NodeName(Child(Child(T,1),1));
         Name2 = NodeName(Child(Child(T,NKids(T)),1));

         if (Name1 != Name2)
         {
           ErrorHeader(T);
           printf ("PROGRAM NAMES DO NOT MATCH\n");
           printf ("\n");
         }

         for (Kid = 2; Kid <= NKids(T)-1; Kid++)
            ProcessNode (Child(T,Kid));
		 /*
		 PrintTree(stdout,RootOfTree(1));
         PrintAllStrings(stdout);
		 */
		 CloseScope();
         break;
		
         case StringNode : 
		 	
            break;
		
         case LoopNode : 
            OpenScope();
   		 	DTEnter(LOOP_CTXT,T,T);
            for (Kid = 1; Kid <= NKids(T); Kid++)
               ProcessNode (Child(T,Kid));
            CloseScope();
			if (Decoration(T) == 0)
			{
                printf ("\nWarning: No 'exit' in 'loop' construct.\n");
			}
            break;	 
			
        case ExitNode : 
			Temp = Lookup(LOOP_CTXT,T);
			if (NodeName(Temp) != LoopNode) {
				ErrorHeader(T);
				printf ("'exit' not in a 'loop' construct.\n");
			}
			Decorate(T,Temp);
			Decorate(Temp,T);
            break;		
			
			
		case CaseNode : 
        Type1 = Expression (Child(T,1)); /*type of Case's expression*/
		/*
		printf("\n\nIn case node, Expression is of type %d\n\n",Type1);
		Decorate(T,Type1);
		*/
		/*
        if (Type1 != TypeInteger)			/*  TODO: Make sure to constrain case literals to be same type as expression in future. */
        /*{
           ErrorHeader(T);
           printf ("Case's Expression should be of type integer!\n");
           printf ("\n");
        }
		*/
        for (Kid = 2; Kid < NKids(T); Kid++) {
			Type2 = Expression(Child(Child(T,Kid),1)); /*Expression on Case_Literal (can be integer, char or identifier, or a range of these) */
			/*
			printf("\n\nNodename of case label is %d \n\n",NodeName(Child(Child(T,Kid),1)));
			*/
			if(NodeName(Child(Child(T,Kid),1)) == IdentifierNode) {
				/*
				printf("\n\nCase literal is Identifier and is a %d!\n\n", NodeName(Decoration(Child(Decoration(Child(Child(T,Kid),1)),1))));
				*/
				if(NodeName(Decoration(Child(Decoration(Child(Child(T,Kid),1)),1))) != LitNode && NodeName(Decoration(Child(Decoration(Child(Child(T,Kid),1)),1))) != ConstNode) {
					ErrorHeader(Child(Child(T,Kid),1));
					printf ("case label must be const or lit!\n");
				}
			}
           ProcessNode (Child(Child(T,Kid),2)); /*  Process statements of case_clauses */
		   /*
		   printf("\n\n%d Expression is of type %d\n\n",Kid, Type1);
		   */
		   if(Type1 != Type2) {
			ErrorHeader(Child(Child(T,Kid),1));
			printf ("Case literals should be the same type as the case expression.\n");
		   }
	   }
		if (NodeName(Child(T,NKids(T))) == CaseClauseNode) {
			Type2 = Expression(Child(Child(T,Kid),1)); /*Expression on Case_Literal (can be integer, char or identifier, or a range of these) */
			if(NodeName(Child(Child(T,Kid),1)) == IdentifierNode) {
				/*
				printf("\n\nCase literal is Identifier!\n\n");
				printf("\n\nCase literal is Identifier and is a %d!\n\n", NodeName(Decoration(Decoration(Child(Child(T,Kid),1)))));
				*/
				if(NodeName(Decoration(Child(Decoration(Child(Child(T,Kid),1)),1))) != LitNode && NodeName(Decoration(Child(Decoration(Child(Child(T,Kid),1)),1))) != ConstNode) {
					ErrorHeader(Child(Child(T,Kid),1));
					printf ("case label must be const or lit!\n");
				}
			}
			ProcessNode (Child(Child(T,NKids(T)),2));	/*  Process statement of last case_clause (when there is no otherwise clause defined) */
 		   if(Type1 != Type2) {
 			ErrorHeader(Child(Child(T,Kid),1));
 			printf ("Case literals should be the same type as the case expression.\n");
 		   }
		} else if (NodeName(Child(T,NKids(T))) == OtherwiseNode) {
			ProcessNode (Child(Child(T,NKids(T)),1));	/*  Process statement of otherwise clause defined */
		}
	      break;			


      case TypesNode :  
         for (Kid = 1; Kid <= NKids(T); Kid++)
            ProcessNode (Child(T,Kid));
         break;


      case TypeNode :         
	  /*printf ("\n\nIn type node of %d\n\n", NodeName(Child(Child(T,1),1)));*/
		 DTEnter (NodeName(Child(Child(T,1), 1)), Child(T, 1), T); 
		 Decorate (Child(Child(T, 1), 1), T); /*decorate node under <id> node with integer/boolean/char/Color with 'type'*/
		 /*The below code is to define how to decorate the <id> nodes and its siblings (if applicable) with type */
		 if(NKids(T) == 1) /*For integer and char*/ {
			/* printf ("\n1\n");*/
			 Decorate (Child(T, 1), T); 
		 }
		 else /*Two children under type node*/
			 /*For enumerations, second child is litnode*/
		 {
		 	 if(NKids(T) == 2 && NodeName(Child(T, 2)) == LitNode) { 
			/*	 printf ("\n2\n");*/
			 Decorate (Child(T,1), T);
		     for (Kid = 1; Kid <= NKids(Child(T,2)); Kid++){
            	 Decorate (Child(Child(T,2), Kid), T); /*Decorate <id> of enum with 'type'*/
				 DTEnter (NodeName(Child(Child(Child(T,2), Kid), 1)),Child(Child(T,2),Kid),Child(T,2));
				 Decorate (Child(Child(Child(T,2), Kid), 1), Child(T, 2));
			 }
		 	} /* For synonyms, /*Second child is identifier */
		 	else if(NKids(T) == 2 && NodeName(Child(T,2)) == IdentifierNode){
			/*	printf ("\n3\n");*/
		 	 ConstDeclareIdNode = Child(T,1);
		     ConstDeclareNameNode = Child(ConstDeclareIdNode,1);
			 ConstValueIdNode = Child(T,2);
			 ConstValueName = NodeName(Child(ConstValueIdNode,1));
			 
			 DTEnter(ConstDeclareNameNode, ConstDeclareIdNode); /*DTEnter the name of new synonym to point to it's <id> declaration*/
			 Decorate(ConstDeclareNameNode, T); /*The name of the synonym will point to the 'type' node, which is the mode of the 'name' */
				 
			 /*Infer type of the const value*/	 
			Decl = Lookup(ConstValueName, T);
			 Type = Decoration(Decl);
			 Decorate(ConstDeclareIdNode, Type); /*The id of the synonym will point to the 'type' node, which is the mode of the 'name' */
			 Decorate(ConstValueIdNode, Decl); /*The <id> of the type should point to the <id> of where the type is first declared.*/
			/*Decorate <id> in declaration with the type, here a pointer to the typeNode above the inferred id's typeDeclaration.*/
		    /*Check mode for when the second child of 'type' is <id> (for synonyms). Only legal values here is 'type'.*/
			Mode = NodeName(Decoration(Child(Decl,1)));
            if (Mode != TypeNode)
            {
               ErrorHeader(T);
               printf ("Can only assign types to other types!\n");
               printf ("\n");
            }
		 }
	 }
         break;
		 
       case ConstsNode :  
            for (Kid = 1; Kid <= NKids(T); Kid++)
               ProcessNode (Child(T,Kid));
			/*
			PrintDclnTable(stdout);
            */
			break;	
			
			
			
	  case ConstNode :			
			constNode = T;
			
			idFirstChildNode = Child(T, 1);
			idSecondChildNode = Child(T, 2);
			
			
			idNameFirstGrandChildNode = Child(idFirstChildNode, 1);
			idNodeNameFirstGrandChildNode = NodeName(idNameFirstGrandChildNode);
			
			/*Below code is to infer type and setup the decorations for the <id> node's type.*/
			if (NodeName(idSecondChildNode) == IntegerNode) {
				intNode = idSecondChildNode;
				Temp = Child (Child (RootOfTree(1), 2), 1); /*Get first intrinsic type node (the one above integer's <id>).*/
				Decorate(idFirstChildNode,Temp); /*Setup type*/
				/*Decorate <id> in declaration with the type, here a pointer to the typeNode above the integer id's typeDeclaration.*/
			} else if  (NodeName(idSecondChildNode) == CharNode) {
				charNode = idSecondChildNode;
				Temp = Child (Child (RootOfTree(1), 2), 3); /*Get third intrinsic type node (the one above char's <id>).*/
				Decorate(idFirstChildNode,Temp); /*Setup type*/
				/*Decorate <id> in declaration with the type, here a pointer to the typeNode above the char id's typeDeclaration.*/
			} else if (NodeName(idSecondChildNode) == IdentifierNode) {
			  	idNameSecondGrandChildNode = Child(idSecondChildNode, 1);
				idNodeNameSecondGrandChild = NodeName(idNameSecondGrandChildNode);
				Decl = Lookup(idNodeNameSecondGrandChild, T); /*For constants already defined OR literals (true,false) */
				Type = Decoration(Decl);
				Decorate(idFirstChildNode,Type); /*Setup type*/
				/*Decorate <id> in declaration with the type, here a pointer to the typeNode above the inferred id's typeDeclaration.*/
			    /*Check mode for when the second child of const is <id>. Only legal values here are const and lit.*/
				Mode = NodeName(Decoration(Child(Decl,1)));
	            if (Mode != ConstNode && Mode != LitNode)
	            {
	               ErrorHeader(T);
	               printf ("Cannot assign variables or types to constants!\n");
	               printf ("\n");
	            }
				Decorate(idSecondChildNode,Decl);
			}
			
			/*
			printf ("\nGoing to do DTEnter for name: %d pointing to %d\n", idNodeNameFirstGrandChildNode, NodeName(idFirstChildNode));
			*/
			DTEnter (idNodeNameFirstGrandChildNode, idFirstChildNode, T); /*Entry in DT for the name pointing to <id> declaration*/
			/*
			printf("\nThe DeclnTable is now;\n");
			            PrintDclnTable(stdout);
			*/			
			Decorate(idNameFirstGrandChildNode, constNode); /*Setup mode*/ /*Pointing to 'const' node from the const's variable name (lValue) */
				
			break;			 


      case DclnsNode :
	  /*printf("\nIn declarations!\n");*/
         for (Kid = 1; Kid <= NKids(T); Kid++) {
			 /*printf("\nIn going to process child %d!\n",Kid);*/
            ProcessNode (Child(T,Kid));
		}
		
		/*printf("\nThe DeclnTable is now;\n");
		            PrintDclnTable(stdout);
		*/				
         break;


      case VarNode :
	  
	  /*printf("\nIn the varNode %d with %d kids\n",T, NKids(T));
	  	  
	  printf("\n1\n");
	  
	  PrintTree(stdout,RootOfTree(1));
		*/  
	  VarTypeIdNode = Child(T, NKids(T));
	  /*printf("\n2, varTypeIdNode is %d\n",VarTypeIdNode);*/
	  VarTypeNameNode = Child(VarTypeIdNode, 1);
      /*printf("\n3\n");*/
	  VarTypeName = NodeName (VarTypeNameNode);
	  
/*	  printf("\nVar tree of %d with id:%d and name:%d\n", VarTypeName, VarTypeIdNode, VarTypeNameNode);*/
	  
	  Type1 = Lookup (VarTypeName,VarTypeIdNode);
	  
/*	  printf("\n<id> lookup of %d is:%d\n", VarTypeName, Type1);*/
	  
	  
	  Decorate(VarTypeIdNode, Type1); /*We set <id> in usage to <id> in declaration to check*/
	  
/*	  printf("\nAfter trying to decorate <id> of %d WITH %d\n", VarTypeName, Type1);*/
	  
	  VarTypeNode = Decoration(Type1); /*Set type of all variable <id>s to this type*/
	  
/*	  printf("\nDecoration of %d is:%d\n", Type1, VarTypeNode);*/
	  
      if (NodeName(VarTypeNode) != TypeNode)
      {
         ErrorHeader(T);
         printf ("Variables can be only be declared with a type (no const/var/literal)!\n");
         printf ("\n");
      }

         for (Kid  = 1; Kid < NKids(T); Kid++)
         {
			Temp = Child(T,Kid);
            DTEnter (NodeName(Child(Temp,1)), Temp, T);
            Decorate (Temp, VarTypeNode);
			Decorate(Child(Temp,1),T);
         }
         break;


      case BlockNode :
         for (Kid = 1; Kid <= NKids(T); Kid++)
            ProcessNode (Child(T,Kid));
         break;


      case AssignNode :
	  	
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));
		 /*
		 printf("\n\nIn AssignNode, the types are %d and %d \n\n", NodeName(Type1), NodeName(Type2));
		 */
		 LeftHandId = Child(T,1);
		 RightHandId = Child(T,2);
		 /*
		 printf("\nMode of leftHandId is %d\n",ModeOf(LeftHandId, IdInUse));
		 */
		 if (ModeOf(LeftHandId, IdInUse) != VarNode) {
             ErrorHeader(Child(T,1));
             printf ("Left hand side of assignment should be a var!\n");
             printf ("\n");
		 }

         if (Type1 != Type2)
         {
            ErrorHeader(T);
            printf ("ASSIGNMENT TYPES DO NOT MATCH\n");
            printf ("\n");
         }
		 
		Decorate(LeftHandId, Lookup(NodeName(Child(LeftHandId,1)),LeftHandId));
		 /* TODO: why was this added? there won't be any entry for a integer literal in DT. this gave false error in pr3.e01. AND integers cant be a type!
		if (NodeName(RightHandId) == IntegerNode) { /*In some cases it could be other functions like not, succ etc.*/
   		 /*
   		 printf("\nMode of rightHandId is %d\n",ModeOf(RightHandId, IdInUse));
   		 */
		/*
   		 if (ModeOf(RightHandId, IdInUse) == TypeNode) {
                ErrorHeader(Child(T,1));
                printf ("Right hand side of assignment cannot be a type!\n");
                printf ("\n");
   		 }
		*/
			/*
			printf("\n\nInside integerNode on the rightSide of assign!\n\n");
		 */
		 /* TODO: why was this added? there won't be any entry for a integer literal in DT. this gave false error in pr3.e01.
			Decorate(RightHandId, Lookup(NodeName(Child(RightHandId,1)),RightHandId));
		*/
		 /*
		}
		*/
 	 	Temp = Lookup(FOR_CTXT,T); 		
 			/*   this variable must be different from all enclosing for loops' control variables. */
 			while(NodeName(Temp) != ProgramNode) {
 				if (NodeName(Child(Child(Temp,1),1)) == NodeName(Child(Child(T,1),1))) {
 	                ErrorHeader(T);
 	                printf ("Assigned variable must have a different name than enclosing for loops' loop control variables!\n");
 				}
 				Temp = Decoration(Temp); /*  Get parent enclosing for loop */
 			}
         break;
		 
         case SwapNode :
            Type1 = Expression (Child(T,1));
            Type2 = Expression (Child(T,2));

   		 LeftHandId = Child(T,1);
   		 RightHandId = Child(T,2);
		 
   		 if (ModeOf(LeftHandId, IdInUse) != VarNode || ModeOf(RightHandId, IdInUse) != VarNode) {
                ErrorHeader(T);
                printf ("Both sides of swap instruction must be type var!\n");
                printf ("\n");
   		 }

            if (Type1 != Type2)
            {
               ErrorHeader(T);
               printf ("Swap TYPES DO NOT MATCH!\n");
               printf ("\n");
            }
	 	 	Temp = Lookup(FOR_CTXT,T); 		
	 			/*   the two variable must be different from all enclosing for loops' control variables. */
	 			while(NodeName(Temp) != ProgramNode) {
	 				if (NodeName(Child(Child(Temp,1),1)) == NodeName(Child(Child(T,1),1)) || NodeName(Child(Child(Temp,1),1)) == NodeName(Child(Child(T,2),1))) {
	 	                ErrorHeader(T);
	 	                printf ("Swap variables must have different names than enclosing for loops' loop control variables!\n");
	 				}
	 				Temp = Decoration(Temp); /*  Get parent enclosing for loop */
	 			}
            break;	 

	  case ReadNode :
      for (Kid = 1; Kid <= NKids(T); Kid++) {
         if (Expression (Child(T,Kid)) != TypeInteger && Expression (Child(T,Kid)) != TypeChar)
         {
            ErrorHeader(T);
            printf ("Read vars MUST BE TYPE INTEGER or CHAR\n");
            printf ("\n");
         }
		 if (ModeOf(Child(T,Kid), IdInUse) != VarNode) {
	          ErrorHeader(Child(T,1));
	          printf ("Identifiers in Read should be of type var!\n");
	          printf ("\n");
		 }
 	 	Temp = Lookup(FOR_CTXT,T); 		
 			/*   the read variable must be different from all enclosing for loops' control variables. */
 			while(NodeName(Temp) != ProgramNode) {
 				if (NodeName(Child(Child(Temp,1),1)) == NodeName(Child(Child(T,Kid),1))) {
 	                ErrorHeader(T);
 	                printf ("Read variables must have different names than enclosing for loops' loop control variables!\n");
 				}
 				Temp = Decoration(Temp); /*  Get parent enclosing for loop */
 			}
 		}
	           break;

      case OutputNode :
	  
         for (Kid = 1; Kid <= NKids(T); Kid++) {
			 if(NodeName(Child(T,Kid)) != StringNode) {
			Type1 = Expression (Child(T,Kid));
            if (Type1 != TypeInteger && Type1 != TypeChar)
            {
               ErrorHeader(T);
               printf ("OUTPUT EXPRESSION MUST BE TYPE INTEGER or CHAR or STRING\n");
               printf ("\n");
            }
		}
			/*
            if (Expression (Child(T,Kid)) != TypeInteger && Expression (Child(T,Kid)) != TypeChar)
            {
               ErrorHeader(T);
               printf ("OUTPUT EXPRESSION MUST BE TYPE INTEGER or CHAR\n");
               printf ("\n");
            }
			*/
         }
            
			
		 break;


      case IfNode :
         if (Expression (Child(T,1)) != TypeBoolean)
         {
            ErrorHeader(T);
            printf ("CONTROL EXPRESSION OF 'IF' STMT");
            printf (" IS NOT TYPE BOOLEAN\n");
            printf ("\n");
         } 

         ProcessNode (Child(T,2));
         if (Rank(T) == 3)
            ProcessNode (Child(T,3));
         break;


      case WhileNode :
         if (Expression (Child(T,1)) != TypeBoolean)
         {
            ErrorHeader(T);
            printf ("WHILE EXPRESSION NOT OF TYPE BOOLEAN\n");
            printf ("\n");
         }
         ProcessNode (Child(T,2));
         break;
	
	case ForDowntoNode :	 
     case ForUptoNode :
	 	Temp = Lookup(FOR_CTXT,T);
		Decorate(T,Temp);
		OpenScope();
		 DTEnter(FOR_CTXT,T,T);
		 DTEnter(LOOP_CTXT,T,T);	/*  disallows exit. */
		 /*  	Process kids   assume <id> has correct type. */
            if (Expression (Child(T,1)) != Expression (Child(T,2)) || Expression (Child(T,1)) != Expression (Child(T,3)))
            {
               ErrorHeader(T);
               printf ("Initial and final values must have same type as loop control variable!\n");
            }
			
			ProcessNode(Child(T,4));
						
			/*   this forUpto's control variable must be different from all enclosing for loops. */
			
			while(NodeName(Temp) != ProgramNode) {
				
				if (NodeName(Child(Child(Temp,1),1)) == NodeName(Child(Child(T,1),1))) {
	                ErrorHeader(T);
	                printf ("Enclosing for loops must have different loop control variables!\n");
				}
				Temp = Decoration(Temp); /*  Get parent's enclosing for loop */
			}
			CloseScope();
            break;	 
		 
         case RepeatNode :
            if (Expression (Child(T,NKids(T))) != TypeBoolean)
            {
               ErrorHeader(T);
               printf ("Repeat's EXPRESSION NOT OF TYPE BOOLEAN!\n");
               printf ("\n");
            }
			if(NKids(T) > 1) {
				for(Kid=1;Kid<NKids(T);Kid++) {
					ProcessNode (Child(T,Kid));
				}
			}
            break;


      case NullNode : 
         break;

      default :
         ErrorHeader(T);
         printf ("UNKNOWN NODE NAME ");
         Write_String (stdout,NodeName(T));
         printf ("\n");

   }  /* end switch */
}  /* end ProcessNode */
