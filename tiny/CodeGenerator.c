/*******************************************************************
          Copyright (C) 1986 by Manuel E. Bermudez
          Translated to C - 1991
********************************************************************/

#include <stdio.h>
#include <header/CommandLine.h>
#include <header/Open_File.h>
#include <header/Table.h>
#include <header/Text.h>
#include <header/Error.h>
#include <header/String_Input.h>
#include <header/Tree.h>
#include <header/Code.h>
#include <header/CodeGenerator.h>  
#define LeftMode 0
#define RightMode 1

    /*  ABSTRACT MACHINE OPERATIONS  */
#define    NOP          1   /* 'NOP'       */
#define    HALTOP       2   /* 'HALT'      */      
#define    LITOP        3   /* 'LIT'       */
#define    LLVOP        4   /* 'LLV'       */
#define    LGVOP        5   /* 'LGV'       */
#define    SLVOP        6   /* 'SLV'       */
#define    SGVOP        7   /* 'SGV'       */
#define    LLAOP        8   /* 'LLA'       */
#define    LGAOP        9   /* 'LGA'       */
#define    UOPOP       10   /* 'UOP'       */
#define    BOPOP       11   /* 'BOP'       */     
#define    POPOP       12   /* 'POP'       */  
#define    DUPOP       13   /* 'DUP'       */
#define    SWAPOP      14   /* 'SWAP'      */
#define    CALLOP      15   /* 'CALL'      */
#define    RTNOP       16   /* 'RTN'       */
#define    GOTOOP      17   /* 'GOTO'      */
#define    CONDOP      18   /* 'COND'      */
#define    CODEOP      19   /* 'CODE'      */
#define    SOSOP       20   /* 'SOS'       */
#define    LIMITOP     21   /* 'LIMIT'     */

    /* ABSTRACT MACHINE OPERANDS */

         /* UNARY OPERANDS */
#define    UNOT        22   /* 'UNOT'     */
#define    UNEG        23   /* 'UNEG'     */
#define    USUCC       24   /* 'USUCC'    */
#define    UPRED       25   /* 'UPRED'    */
         /* BINARY OPERANDS */
#define    BAND        26   /* 'BAND'     */
#define    BOR         27   /* 'BOR'      */
#define    BPLUS       28   /* 'BPLUS'    */
#define    BMINUS      29   /* 'BMINUS'   */
#define    BMULT       30   /* 'BMULT'    */
#define    BDIV        31   /* 'BDIV'     */
#define    BEXP        32   /* 'BEXP'     */
#define    BMOD        33   /* 'BMOD'     */
#define    BEQ         34   /* 'BEQ'      */
#define    BNE         35   /* 'BNE'      */
#define    BLE         36   /* 'BLE'      */
#define    BGE         37   /* 'BGE'      */
#define    BLT         38   /* 'BLT'      */
#define    BGT         39   /* 'BGT'      */
         /* OS SERVICE CALL OPERANDS */
#define    TRACEX      40   /* 'TRACEX'   */
#define    DUMPMEM     41   /* 'DUMPMEM'  */
#define    OSINPUT     42   /* 'INPUT'    */
#define    OSINPUTC    43   /* 'INPUT'    */
#define    OSOUTPUT    44   /* 'OUTPUT'   */
#define    OSOUTPUTC   45   /* 'OUTPUT'   */
#define    OSOUTPUTL   46   /* 'OUTPUTL'  */
#define    OSEOF       47   /* 'EOF'      */

         /* TREE NODE NAMES */
#define    ProgramNode  48   /* 'program'  */
#define    TypesNode    49   /* 'types'    */
#define    TypeNode     50   /* 'type'     */
#define    DclnsNode    51   /* 'dclns'    */
#define    DclnNode     52   /* 'dcln'     */
#define    IntegerTNode 53   /* 'integer'  */
#define    BooleanTNode 54   /* 'boolean'  */
#define    BlockNode    55   /* 'block'    */
#define    AssignNode   56   /* 'assign'   */
#define    OutputNode   57   /* 'output'   */ 
#define    IfNode       58   /* 'if'       */
#define    WhileNode    59   /* 'while'    */
#define    NullNode     60   /* '<null>'   */
#define    LENode       61   /* '<='       */
#define    PlusNode     62   /* '+'        */
#define    MinusNode    63   /* '-'        */
#define    ReadNode     64   /* 'read'     */
#define    IntegerNode  65   /* '<integer>'*/
#define    IdentifierNode 66 /* '<identifier>'*/
#define ExponentiationNode 67	/* '**' */
#define NotNode 68				/* 'not' */
#define OrNode 69				/* 'or' */
#define MultNode 70				/* '*' */
#define DivNode 71				/* '/' */
#define AndNode 72				/* 'and' */
#define ModNode 73				/* 'mod' */
#define EqNode 74				/* '=' */
#define NotEqNode 75				/* '<>' */
#define GTENode 76				/* '>=' */
#define LTNode 77				/* '<' */
#define GTNode 78				/* '>' */
#define TrueNode 79				/* 'true' */
#define FalseNode 80			/* 'false' */
#define EOFNode 81				/* 'eof' */
#define RepeatNode 82				/* 'repeat' */
#define SwapNode 83			/* 'swap' */
#define LoopNode 84			/* 'loop' */
#define ExitNode 85			/* 'exit' */
#define ForUptoNode 86			/* 'upto' */
#define ForDowntoNode 87			/* 'downto' */
#define CaseNode 88			/* 'case' */
#define RangeNode 89			/* '..' */
#define CaseClauseNode 90			/* case_clause */
#define OtherwiseNode 91		/* otherwise */
#define LitNode 92
#define CharTNode 93
#define CharNode 94
#define ConstsNode 95
#define ConstNode 96
#define VarNode 97
#define StringNode 98
#define SuccNode 99
#define PredNode 100
#define OrdNode 101
#define ChrNode 102



#define    NumberOfNodes 102
typedef int Mode;

FILE *CodeFile;
char *CodeFileName;
Clabel HaltLabel;

char *mach_op[] = 
    {"NOP","HALT","LIT","LLV","LGV","SLV","SGV","LLA","LGA",
     "UOP","BOP","POP","DUP","SWAP","CALL","RTN","GOTO","COND",
     "CODE","SOS","LIMIT","UNOT","UNEG","USUCC","UPRED","BAND",
     "BOR","BPLUS","BMINUS","BMULT","BDIV","BEXP","BMOD","BEQ",
     "BNE","BLE","BGE","BLT","BGT","TRACEX","DUMPMEM","INPUT",
     "INPUTC","OUTPUT","OUTPUTC","OUTPUTL","EOF"};

/****************************************************************** 
   add new node names to the end of the array, keeping in strict order
   as defined above, then adjust the j loop control variable in
   InitializeNodeNames(). 
*******************************************************************/
char *node_name[] =
    {"program","types","type","dclns","dcln","integer",
     "boolean","block","assign","output","if","while",
     "<null>","<=","+","-","read","<integer>","<identifier>","**","not","or","*",
 	"/","and","mod","=","<>",">=","<",">","true","false","eof","repeat","swap","loop",
	"exit", "upto", "downto", "case", "..", "case_clause", "otherwise", "lit", "char",
	"<char>", "consts", "const", "var", "<string>", "succ", "pred", "ord", "chr"};


void CodeGenerate(int argc, char *argv[])
{
   int NumberTrees;
   /*
   printf("\nIn CodeGenerate!\n");
   */
   InitializeCodeGenerator(argc,argv);
   Tree_File = Open_File("_TREE", "r"); 
   NumberTrees = Read_Trees();
   fclose (Tree_File);                     
   /*
   printf("\nBefore ProcessNode in CodeGenerate!\n");
*/
   HaltLabel = ProcessNode (RootOfTree(1), NoLabel);
   /*
   printf("\nAfter ProcessNode in CodeGenerate!\n");
   */
   CodeGen0 (HALTOP, HaltLabel); 

   CodeFile = Open_File (CodeFileName, "w");
   DumpCode (CodeFile);
   fclose(CodeFile); 

   if (TraceSpecified)
      fclose (TraceFile);

/****************************************************************** 
  enable this code to write out the tree after the code generator
  has run.  It will show the new decorations made with MakeAddress().
*******************************************************************/


   Tree_File = fopen("_TREE", "w");  
   Write_Trees();
   fclose (Tree_File);                   
}


void InitializeCodeGenerator(int argc,char *argv[])
{
   InitializeMachineOperations();
   InitializeNodeNames();
   FrameSize = 0;
   CurrentProcLevel = 0;
   LabelCount = 0;
   CodeFileName = System_Argument("-code", "_CODE", argc, argv); 
}


void InitializeMachineOperations(void)
{
   int i,j;

   for (i=0, j=1; i < 47; i++, j++)
      String_Array_To_String_Constant (mach_op[i],j);
}



void InitializeNodeNames(void)
{
   int i,j;

   for (i=0, j=48; j <= NumberOfNodes; i++, j++)
      String_Array_To_String_Constant (node_name[i],j);
}



String MakeStringOf(int Number)
{
   Stack Temp;

   Temp = AllocateStack (50);
   ResetBufferInTextTable();
   if (Number == 0)
      AdvanceOnCharacter ('0');
   else
   {
      while (Number > 0)
      {
         Push (Temp,(Number % 10) + 48);
         Number /= 10;
      }

      while ( !(IsEmpty (Temp)))
         AdvanceOnCharacter ((char)(Pop(Temp)));
   }   
   return (ConvertStringInBuffer()); 
}  



void Reference(TreeNode T, Mode M, Clabel L)
{
   int Addr,OFFSET, IdMode;
   String  Op; 
   Addr = Decoration(Decoration(T));
   switch (M)
   {
      case LeftMode  :  
                        if (ProcLevel (Addr) == 0) 
                           Op = SGVOP;
                        else
                           Op = SLVOP;
					    OFFSET = FrameDisplacement (Addr) ;
						CodeGen1 (Op,MakeStringOf(OFFSET),L);
						DecrementFrameSize();
	                break;
      case RightMode :  	  
	IncrementFrameSize();
      if (ProcLevel (Addr) == 0) 
         Op = LGVOP;
  		else
         Op = LLVOP;
      IdMode = NodeName(Decoration(Child(Decoration(T),1)));
    		 if (IdMode == ConstNode || IdMode == LitNode) {
    		  OFFSET  = Decoration(Decoration(T));
			  CodeGen1 (LITOP, OFFSET, L);
		 } else {
		 	  OFFSET = FrameDisplacement (Addr) ;
			  CodeGen1 (Op,MakeStringOf(OFFSET),L);
		 }
                        break;
   }
}



int NKids (TreeNode T)
{
   return (Rank(T));
}


void Expression (TreeNode T, Clabel CurrLabel)
{
   TreeNode Decl;
   int Kid, Mode, Value;
   unsigned int CharToInt;
   Clabel Label1;

   if (TraceSpecified)
   {
      fprintf (TraceFile, "<<< CODE GENERATOR >>> Processing Node ");
      Write_String (TraceFile, NodeName (T) );
      fprintf (TraceFile, " , Label is  ");
      Write_String (TraceFile, CurrLabel);
      fprintf (TraceFile, "\n");
   }

   switch (NodeName(T))
   {
      case ExponentiationNode :
      	Expression ( Child(T,1) , CurrLabel);
      	Expression ( Child(T,2) , NoLabel);
      	CodeGen1 (BOPOP, BEXP, NoLabel);
      	DecrementFrameSize();
      	break;
	
        case TrueNode :
		IncrementFrameSize();
        CodeGen1 (LITOP, MakeStringOf(1), CurrLabel); 
        break;	
		
        case FalseNode :
        IncrementFrameSize();
        CodeGen1 (LITOP, MakeStringOf(0), CurrLabel); 
        break;	
			
	        case GTNode :
	        	Expression ( Child(T,1) , CurrLabel);
	        	Expression ( Child(T,2) , NoLabel);
	        	CodeGen1 (BOPOP, BGT, NoLabel);
	        	DecrementFrameSize();
	        	break;				
	
        case LENode :
        	Expression ( Child(T,1) , CurrLabel);
        	Expression ( Child(T,2) , NoLabel);
        	CodeGen1 (BOPOP, BLE, NoLabel);
        	DecrementFrameSize();
        	break;
			
	        case EqNode :
	        	Expression ( Child(T,1) , CurrLabel);
	        	Expression ( Child(T,2) , NoLabel);
	        	CodeGen1 (BOPOP, BEQ, NoLabel);
	        	DecrementFrameSize();
	        	break;
				
		        case NotEqNode :
		        	Expression ( Child(T,1) , CurrLabel);
		        	Expression ( Child(T,2) , NoLabel);
		        	CodeGen1 (BOPOP, BNE, NoLabel);
		        	DecrementFrameSize();
		        	break;
					
			        case GTENode :
			        	Expression ( Child(T,1) , CurrLabel);
			        	Expression ( Child(T,2) , NoLabel);
			        	CodeGen1 (BOPOP, BGE, NoLabel);
			        	DecrementFrameSize();
			        	break;
						
				        case LTNode :
				        	Expression ( Child(T,1) , CurrLabel);
				        	Expression ( Child(T,2) , NoLabel);
				        	CodeGen1 (BOPOP, BLT, NoLabel);
				        	DecrementFrameSize();
				        	break;																	
			
			
	        case MultNode :
	        	Expression ( Child(T,1) , CurrLabel);
	        	Expression ( Child(T,2) , NoLabel);
	        	CodeGen1 (BOPOP, BMULT, NoLabel);
	        	DecrementFrameSize();
	        	break;		
				
		        case DivNode :
		        	Expression ( Child(T,1) , CurrLabel);
		        	Expression ( Child(T,2) , NoLabel);
		        	CodeGen1 (BOPOP, BDIV, NoLabel);
		        	DecrementFrameSize();
		        	break;		
					
			        case AndNode :
			        	Expression ( Child(T,1) , CurrLabel);
			        	Expression ( Child(T,2) , NoLabel);
			        	CodeGen1 (BOPOP, BAND, NoLabel);
			        	DecrementFrameSize();
			        	break;		
	
				        case ModNode :
				        	Expression ( Child(T,1) , CurrLabel);
				        	Expression ( Child(T,2) , NoLabel);
				        	CodeGen1 (BOPOP, BMOD, NoLabel);
				        	DecrementFrameSize();
				        	break;		
							
							
					        case OrNode :
					        	Expression ( Child(T,1) , CurrLabel);
					        	Expression ( Child(T,2) , NoLabel);
					        	CodeGen1 (BOPOP, BOR, NoLabel);
					        	DecrementFrameSize();
					        	break;									
		
      case PlusNode :
      Expression ( Child(T,1) , CurrLabel);
      if (Rank(T) == 2)
      {
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BPLUS, NoLabel);
         DecrementFrameSize();
      }
      else
         CodeGen1 (UOPOP, NOP, NoLabel);
      break;

      case NotNode :
         Expression ( Child(T,1) , CurrLabel);
         CodeGen1 (UOPOP, UNOT, NoLabel);
         break;

      case MinusNode :
         Expression ( Child(T,1) , CurrLabel);
         if (Rank(T) == 2)
         {
            Expression ( Child(T,2) , NoLabel);
            CodeGen1 (BOPOP, BMINUS, NoLabel);
            DecrementFrameSize();
         }
         else
            CodeGen1 (UOPOP, UNEG, NoLabel);
         break;
		 
         case EOFNode :
         IncrementFrameSize();
            CodeGen1 (SOSOP, OSEOF, CurrLabel);
            break;	 

      case IntegerNode :
	  	IncrementFrameSize();
         CodeGen1 (LITOP, NodeName (Child(T,1)), CurrLabel);
         break;
		 
      case CharNode :
   	  	IncrementFrameSize();
		/*
		printf("\n\nThe char is %c\n\n",Character(NodeName(Child(T, 1)), 2));
		*/
		CodeGen1 (LITOP, MakeStringOf(Character(NodeName(Child(T, 1)), 2)), CurrLabel);
		/*
		CharToInt = (unsigned int) Character(NodeName (Child(T,1)),2); /*Convert to unsigned int*/
		/*
			CodeGen1 (LITOP, MakeStringOf(CharToInt), CurrLabel);
			*/
            break;	 
		 
         case StringNode :
   	  	IncrementFrameSize();
            CodeGen1 (LITOP, NodeName (Child(T,1)), CurrLabel);
            break;	 

      case IdentifierNode :
         Reference (T,RightMode,CurrLabel);
         break;

   	  case SuccNode:
   	  case PredNode:
	  	Expression(Child(T,1), CurrLabel);
		if (NodeName(T) == SuccNode) {
			CodeGen1 (UOPOP, USUCC, NoLabel);
		} else {
			CodeGen1 (UOPOP, UPRED, NoLabel);
		}
	  	if (NKids(Decoration(Decoration(Child(T,1)))) == 2) {
			IncrementFrameSize();
	  		CodeGen1 (LITOP, MakeStringOf(0), NoLabel);
			IncrementFrameSize();
			CodeGen1 (LITOP, MakeStringOf(NKids(Child(Decoration(Decoration(Child(T,1))),2))), NoLabel);
			CodeGen0 (LIMITOP,  NoLabel);
	  	}
	  	break;
   	  case OrdNode:
	  	Expression(Child(T,1), CurrLabel);
   	  	break;
	  
   	  case ChrNode:
	  	Expression(Child(T,1), CurrLabel);
   	   break;
	   
      default :
         ReportTreeErrorAt(T);
         printf ("<<< CODE GENERATOR >>> : UNKNOWN NODE NAME ");
         Write_String (stdout,NodeName(T));
         printf ("\n");

   } /* end switch */
} /* end Expression */



Clabel ProcessNode (TreeNode T, Clabel CurrLabel)
{
   int Kid, Num, Type;
   Clabel Label1, Label2, Label3, CascadeLabel, ExitLabel, ThisLabel, NextLabel;
   TreeNode VariableNode;

   if (TraceSpecified)
   {
      fprintf (TraceFile, "<<< CODE GENERATOR >>> Processing Node ");
      Write_String (TraceFile, NodeName (T) );
      fprintf (TraceFile, " , Label is  ");
      Write_String (TraceFile, CurrLabel);
      fprintf (TraceFile, "\n");
   }

   switch (NodeName(T))
   {
      case ProgramNode :
	  /*
	  printf("\nIn Program Node!\n");
	  */
	  PrintAllStrings(stdout);
         CurrLabel = ProcessNode (Child(T,2),CurrLabel);
		 for(Kid=3;Kid<NKids(T);Kid++) {
         CurrLabel = ProcessNode (Child(T,Kid),CurrLabel);		 
	 }
         return (CurrLabel);

      case TypesNode :
         for (Kid = 1; Kid <= NKids(T); Kid++)
            CurrLabel = ProcessNode (Child(T,Kid), CurrLabel);
         return (CurrLabel);

      case TypeNode :	  
	 if(NKids(T) == 2 && NodeName(Child(T,2)) == LitNode) {
  		 ProcessNode(Child(T,2), NoLabel);
  	 }
	     return (CurrLabel);


      case DclnsNode :
	   for (Kid = 1; Kid <= NKids(T); Kid++)
           CurrLabel = ProcessNode (Child(T,Kid), CurrLabel);
  PrintTree(stdout,RootOfTree(1));	
         if (NKids(T) > 0)
            return (NoLabel);
         else
            return (CurrLabel);

      case VarNode :
	  
         for (Kid = 1; Kid < NKids(T); Kid++)
         {
            Num = MakeAddress();
            Decorate ( Child(T,Kid), Num);
            IncrementFrameSize();
			CodeGen1 (LITOP, MakeStringOf(0), NoLabel);
         }
         return (NoLabel);                 

      case BlockNode :
         for (Kid = 1; Kid <= NKids(T); Kid++)
            CurrLabel = ProcessNode (Child(T,Kid), CurrLabel);
         return (CurrLabel); 


      case AssignNode :
         Expression (Child(T,2), CurrLabel);
         Reference (Child(T,1), LeftMode, NoLabel);
         return (NoLabel);
		 
     case SwapNode :
		 	Reference (Child(T,1), RightMode, CurrLabel);
            Reference (Child(T,2), RightMode, CurrLabel);
            Reference (Child(T,1), LeftMode, NoLabel);
			Reference (Child(T,2), LeftMode, NoLabel);
            return (NoLabel);	 


			case OutputNode: 
			                for (Kid = 1; Kid <= NKids(T); Kid++) {
								if(NodeName(Child(T, Kid)) == IdentifierNode && NodeName(Decoration(Child(Decoration(Child(T, Kid)), 1))) != LitNode)
											                        {
											                                Expression (Child(T,Kid), CurrLabel);

											                                if(Decoration(Decoration(Child(Decoration(Child(Decoration(Child(T, Kid)), 1)), NKids(Decoration(Child(Decoration(Child(T, Kid)), 1)))))) == Child (Child (RootOfTree(1), 2), 3))
											                                        CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);
											                                else
											                                        CodeGen1 (SOSOP, OSOUTPUT, NoLabel);
											                                DecrementFrameSize();
											                                CurrLabel = NoLabel;
											                        }
											                        else if(Decoration(Child(T, Kid)) == Child (Child (RootOfTree(1), 2), 3) || NodeName(Child(T, Kid)) == CharNode)
											                        {
											                                Expression (Child(T,Kid), CurrLabel);
											                                CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);
											                                DecrementFrameSize();
											                                CurrLabel = NoLabel;
											                        }
			                        
			                        else if(NodeName(Child(T, Kid)) == StringNode){
										/*Iterate through every character for string*/
			                                Num = 2; /*Ignore first " quote*/
			                                while(Character(NodeName(Child(Child(T, Kid), 1)), Num) != '"'){
			                                    if(Character(NodeName(Child(Child(T, Kid), 1)), Num) == '\\'){
			                                            Num++;
			                                            switch(Character(NodeName(Child(Child(T, Kid), 1)), Num)){
			                                                    case 'a':   
																	CodeGen1 (LITOP, MakeStringOf('\a'), CurrLabel); 
																	break;
			                                                    case 'b':   
																	CodeGen1 (LITOP, MakeStringOf('\b'), CurrLabel); 
																	break;
			                                                    case 'f':   
																	CodeGen1 (LITOP, MakeStringOf('\f'), CurrLabel); 
																	break;
			                                                    case 'n':   
																	CodeGen1 (LITOP, MakeStringOf('\n'), CurrLabel); 
																	break;
			                                                    case 'r':   
																	CodeGen1 (LITOP, MakeStringOf('\r'), CurrLabel); 
																	break;
			                                                    case 't':   
																	CodeGen1 (LITOP, MakeStringOf('\t'), CurrLabel); 
																	break;
			                                                    case 'v':   
																	CodeGen1 (LITOP, MakeStringOf('\v'), CurrLabel); 
																	break;
			                                                    case '\\':  
																	CodeGen1 (LITOP, MakeStringOf('\\'), CurrLabel); 
																	break;
			                                                    case '\'':  
																	CodeGen1 (LITOP, MakeStringOf('\''), CurrLabel); 
																	break;
			                                                    case '"':   
																	CodeGen1 (LITOP, MakeStringOf('"'), CurrLabel); 
																	break;
			                                                    case '?':   
																	CodeGen1 (LITOP, MakeStringOf('?'), CurrLabel); 
																	break;
			                                                    default:   
																	CodeGen1 (LITOP, MakeStringOf('?'), CurrLabel); 
																	break;
			                                            }
			                                    }
			                                    else
			                                            CodeGen1 (LITOP, MakeStringOf(Character(NodeName(Child(Child(T, Kid), 1)), Num)), CurrLabel);
			                                    IncrementFrameSize();
			                                    CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);
			                                    DecrementFrameSize();
			                                    Num++;
			                                    CurrLabel = NoLabel;
			                                }
			                        }
			                        else
			                        {
			                                Expression (Child(T,Kid), CurrLabel);
			                                CodeGen1 (SOSOP, OSOUTPUT, NoLabel);
			                                DecrementFrameSize();
			                                CurrLabel = NoLabel;
			                        }
								}
			                CodeGen1 (SOSOP, OSOUTPUTL, NoLabel);
			                return (NoLabel);

      case ReadNode :
			/*check type*/
	  		VariableNode = Decoration(Child(Decoration(Child(T,1)),1));
			Type = NodeName(Child(Child(VariableNode, NKids(VariableNode)),1));
            IncrementFrameSize();
			if (Type == IntegerTNode) {
            	CodeGen1 (SOSOP, OSINPUT, CurrLabel);
			} else if (Type == CharTNode) {
				CodeGen1 (SOSOP, OSINPUTC, CurrLabel);
			}
			Reference (Child(T,1), LeftMode, NoLabel);
			/*store to var*/
            for (Kid = 2; Kid <= NKids(T); Kid++)
            {
		  		VariableNode = Decoration(Child(Decoration(Child(T,Kid)),1));
				Type = NodeName(Child(Child(VariableNode, NKids(VariableNode)),1));
	            IncrementFrameSize();
				if (Type == IntegerTNode) {
	            CodeGen1 (SOSOP, OSINPUT, NoLabel);
				} else if (Type == CharTNode) {
				CodeGen1 (SOSOP, OSINPUTC, NoLabel);
				}
				Reference (Child(T,Kid), LeftMode, NoLabel);
            }
            return (NoLabel);

      case IfNode :
         Expression (Child(T,1), CurrLabel);
         Label1 = MakeLabel();
         Label2 = MakeLabel();
         Label3 = MakeLabel();
         CodeGen2 (CONDOP,Label1,Label2, NoLabel);
         DecrementFrameSize();
         CodeGen1 (GOTOOP, Label3, ProcessNode (Child(T,2), Label1) );
         if (Rank(T) == 3)
            CodeGen0 (NOP, ProcessNode (Child(T,3),Label2));
         else
            CodeGen0 (NOP, Label2);
         return (Label3);     
	
	case ForDowntoNode :	 
    case ForUptoNode :
            Expression (Child(T,3), CurrLabel); /*  evaluate F */
			Expression (Child(T,2), NoLabel);	/*  evaluate I */
            Reference (Child(T,1), LeftMode, NoLabel); /*  store to i */
            Label1 = MakeLabel();
			Label2 = MakeLabel();
			Label3 = MakeLabel();
			CodeGen0 (DUPOP, Label1);
			IncrementFrameSize();
			Reference (Child(T,1), RightMode, NoLabel); /*  Load from i */
			if (NodeName(T) == ForUptoNode) {
				CodeGen1 (BOPOP, BGE, NoLabel);
			} else {
				CodeGen1 (BOPOP, BLE, NoLabel);
			}
			DecrementFrameSize();
			CodeGen2 (CONDOP,Label2,Label3, NoLabel);
			DecrementFrameSize();
            CascadeLabel = ProcessNode(Child(T,4),Label2); /*  Process S */
			Reference (Child(T,1), RightMode, CascadeLabel); /*  Load from i */
			if (NodeName(T) == ForUptoNode) {
				CodeGen1 (UOPOP, USUCC, NoLabel);
			} else {
				CodeGen1 (UOPOP, UPRED, NoLabel);
			}
			Reference (Child(T,1), LeftMode, NoLabel); /*  store to i */
            CodeGen1 (GOTOOP, Label1, NoLabel);
            CodeGen1 (POPOP, MakeStringOf(1), Label3);
			DecrementFrameSize();
			IncrementFrameSize();
			CodeGen1 (LITOP, MakeStringOf(0), NoLabel); /*  After loop is done, clear the control variable to zero */
            Reference (Child(T,1), LeftMode, NoLabel); /*  store to i */
            return (NoLabel);  		            

	
	case CaseNode :
		Expression (Child(T,1), CurrLabel); /*  evaluate E */
		ExitLabel = MakeLabel();
		NextLabel = NoLabel;	/*  for L1 within the first CL */
        for (Kid = 2; Kid < NKids(T); Kid++) {
			/*  In the if-else below, calculate the case_literal (CLi) */
           if (NodeName(Child(Child(T,Kid),1)) == IntegerNode) { /*  There is only one case literal, example "1:{S}" */
   			CodeGen0 (DUPOP, NextLabel);
   			IncrementFrameSize();
			CodeGen1 (LITOP, NodeName (Child(Child(Child(T,Kid),1),1)), NoLabel); /*  Put the case literal on top of the stack */
			IncrementFrameSize();
			CodeGen1 (BOPOP, BEQ, NoLabel);
           } else if (NodeName(Child(Child(T,Kid),1)) == IdentifierNode) { /*  There is only one case literal, example "1:{S}" */
   			CodeGen0 (DUPOP, NextLabel);
   			IncrementFrameSize();
			Expression (Child(Child(T,Kid),1), NoLabel); /*  Put the case literal on top of the stack */
			IncrementFrameSize();
			CodeGen1 (BOPOP, BEQ, NoLabel);
           } else if (NodeName(Child(Child(T,Kid),1)) == RangeNode) {	/*  There is range case literal, example "1..5:{S}" */
  			CodeGen0 (DUPOP, NextLabel);
  			IncrementFrameSize();
   			CodeGen0 (DUPOP, NoLabel);
   			IncrementFrameSize();
			CodeGen1 (LITOP, NodeName (Child(Child(Child(Child(T,Kid),1),1),1)), NoLabel); /*  Put the lower case range literal 'l' on top of the stack */
			IncrementFrameSize();
			CodeGen1 (BOPOP, BGE, NoLabel);
			DecrementFrameSize();
			CodeGen0 (SWAPOP, NoLabel);					
			IncrementFrameSize();			
			CodeGen1 (LITOP, NodeName (Child(Child(Child(Child(T,Kid),1),2),1)), NoLabel); /*  Put the upper case range literal 'u' on top of the stack */
			CodeGen1 (BOPOP, BLE, NoLabel);
			DecrementFrameSize();
			CodeGen1 (BOPOP, BAND, NoLabel);
			DecrementFrameSize();
           }
           ThisLabel = MakeLabel();
			NextLabel = MakeLabel();
			CodeGen2 (CONDOP,ThisLabel,NextLabel, NoLabel);
			DecrementFrameSize();
            CodeGen1 (POPOP, MakeStringOf(1), ThisLabel);
			DecrementFrameSize();
			CascadeLabel = ProcessNode(Child(Child(T,Kid),2),NoLabel); /*  Process statement within the case_clause */
			CodeGen1 (GOTOOP, ExitLabel, CascadeLabel);
	   }
	   
	   if(NodeName(Child(T,NKids(T))) == CaseClauseNode) { /*  No 'otherwise' */
			/*  In the if-else below, calculate the case_literal (CLi) */
           if (NodeName(Child(Child(T,NKids(T)),1)) == IntegerNode) { /*  There is only one case literal, example "1:{S}" */
   			CodeGen0 (DUPOP, NextLabel);
   			IncrementFrameSize();
			CodeGen1 (LITOP, NodeName (Child(Child(Child(T,NKids(T)),1),1)), NoLabel); /*  Put the case literal on top of the stack */
			IncrementFrameSize();
			CodeGen1 (BOPOP, BEQ, NoLabel);
           } else if (NodeName(Child(Child(T,NKids(T)),1)) == RangeNode) {	/*  There is range case literal, example "1..5:{S}" */
  			CodeGen0 (DUPOP, NextLabel);
  			IncrementFrameSize();
   			CodeGen0 (DUPOP, NoLabel);
   			IncrementFrameSize();
			CodeGen1 (LITOP, NodeName (Child(Child(Child(Child(T,NKids(T)),1),1),1)), NoLabel); /*  Put the lower case range literal 'l' on top of the stack */
			IncrementFrameSize();
			CodeGen1 (BOPOP, BGE, NoLabel);
			DecrementFrameSize();
			CodeGen0 (SWAPOP, NoLabel);					
			IncrementFrameSize();			
			CodeGen1 (LITOP, NodeName (Child(Child(Child(Child(T,NKids(T)),1),2),1)), NoLabel); /*  Put the upper case range literal 'u' on top of the stack */
			CodeGen1 (BOPOP, BLE, NoLabel);
			DecrementFrameSize();
			CodeGen1 (BOPOP, BAND, NoLabel);
			DecrementFrameSize();
           }
           ThisLabel = MakeLabel();
			NextLabel = MakeLabel();
			CodeGen2 (CONDOP,ThisLabel,NextLabel, NoLabel);
			DecrementFrameSize();
            CodeGen1 (POPOP, MakeStringOf(1), ThisLabel);
			DecrementFrameSize();
			CascadeLabel = ProcessNode(Child(Child(T,NKids(T)),2),NoLabel); /*  Process statement within the case_clause */
			CodeGen1 (GOTOOP, ExitLabel, CascadeLabel);
			
			CodeGen1 (POPOP, MakeStringOf(1), NextLabel);
	   } else if (NodeName(Child(T,NKids(T))) == OtherwiseNode) {
	   		CodeGen1 (POPOP, MakeStringOf(1), NextLabel);
			ProcessNode(Child(Child(T,NKids(T)),1),NoLabel); /*  Process statement within the case_clause */
	   }
	   
		return (ExitLabel);

      case WhileNode :
         if (CurrLabel == NoLabel) 
            Label1 = MakeLabel();
         else 
            Label1 = CurrLabel;
         Label2 = MakeLabel();
         Label3 = MakeLabel();
         Expression (Child(T,1), Label1);
         CodeGen2 (CONDOP, Label2, Label3, NoLabel);
         DecrementFrameSize();
         CodeGen1 (GOTOOP, Label1, ProcessNode (Child(T,2), Label2) );
         return (Label3);
		 
		
      case LoopNode :
			if (CurrLabel == NoLabel) 
               Label1 = MakeLabel();
            else 
               Label1 = CurrLabel;
            Label2 = MakeLabel();
			Decorate(T,Label2); /*  The exit label for this loop node, which is used by the enclosed exit. */
				CascadeLabel = Label1;
	            for (Kid = 1; Kid <= NKids(T); Kid++)
	            {
	               CascadeLabel = ProcessNode (Child(T,Kid),CascadeLabel);
	            }
            CodeGen1 (GOTOOP, Label1, CascadeLabel); /*  After processing the loop's enclosing children statements, go back to start of the loop and repeat all the  enclosing statements.*/
            return (Label2); /*  Label2 is the exit label for the loop construct */
			
	 case ExitNode :
				Label1 = Decoration(Decoration(T)); /*  Get the decoration of the exit (which is its enclosing loop stmt), then get its decoration (which is the exit label for that loop stmt)			 */
				CodeGen1 (GOTOOP, Label1, CurrLabel);
			return(NoLabel);
			
        case RepeatNode :
               if (CurrLabel == NoLabel) 
                  Label1 = MakeLabel();
               else 
                  Label1 = CurrLabel;
               Label2 = MakeLabel();
   			if (NKids(T)>1) {
   				CascadeLabel = ProcessNode (Child(T,1), Label1);
   	            for (Kid = 2; Kid < NKids(T); Kid++)
   	            {
   	               CascadeLabel = ProcessNode (Child(T,Kid),CascadeLabel);
   	            }
   			}

               Expression (Child(T,NKids(T)), CascadeLabel); /*  The nth child would be the until expression */
               CodeGen2 (CONDOP, Label2, Label1, NoLabel);
               DecrementFrameSize();
               return (Label2);				 

	   case LitNode : 
	         for(Kid=1; Kid<=NKids(T); Kid++)
				 Decorate(Child(T,Kid),MakeStringOf(Kid-1));
			 return CurrLabel;
		
  case ConstsNode : 
 	  	  for(Kid=1; Kid<=NKids(T); Kid++)
 	  		ProcessNode(Child(T,Kid), NoLabel);	
 		  return CurrLabel;	 
		  
		  
 	case ConstNode : 	  
 	 if(NodeName(Child(T,2)) == IntegerNode) {
		 /*
		 printf("\n\nConst integer!\n\n");
 		 */
		 Decorate(Child(T,1), NodeName(Child(Child(T,2),1)));
 	 } else if(NodeName(Child(T,2)) == CharNode) {
		 /*
		 printf("\n\nConst char!\n\n");
 		 */
		 Decorate(Child(T,1), MakeStringOf(Character(NodeName(Child(Child(T,2),1)),2)));
 	 } else if(NodeName(Child(T,2)) == IdentifierNode) {
		 /*
		 printf("\n\nConst id!\n\n");
 	 	*/
		Decorate(Child(T,1), Decoration(Lookup(NodeName(Child(Child(T,2),1)),T)));
 	 }
 		return CurrLabel;		

       case NullNode : return(CurrLabel);

 
      default :
              ReportTreeErrorAt(T);
              printf ("<<< CODE GENERATOR >>> : UNKNOWN NODE NAME ");
              Write_String (stdout,NodeName(T));
              printf ("\n");

   } /* end switch */
}   /* end ProcessNode */
