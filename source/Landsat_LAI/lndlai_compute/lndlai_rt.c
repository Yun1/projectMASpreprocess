/*************************************************************************/
/*									 */
/*	Source code for use with Cubist Release 2.02			 */
/*	--------------------------------------------			 */
/*		   Copyright RuleQuest Research 2005			 */
/*									 */
/*	This code is provided "as is" without warranty of any kind,	 */
/*	either express or implied.  All use is at your own risk.	 */
/*									 */
/*************************************************************************/


/*************************************************************************/
/*									 */
/*	Sample program to demonstrate the use of Cubist models		 */
/*	------------------------------------------------------		 */
/*									 */
/*	Compilation:							 */
/*									 */
/*	    Unix: use an ANSI C compiler such as gcc and include	 */
/*		  the math library, e.g. gcc -O2 sample.c -lm		 */
/*									 */
/*	    Windows: compile as a console application with symbol	 */
/*		  "WIN32" defined					 */
/*									 */
/*	The options for this program are:				 */
/*									 */
/*	    -f <filestem>   specify the application name		 */
/*	    -p		    print the cubist model			 */
/*									 */
/*	The program expects to find the following files:		 */
/*									 */
/*	    <filestem>.names (the application names file)		 */
/*	    <filestem>.model (the model file generated by Cubist)	 */
/*	    <filestem>.cases (with the same format as a .data file) 	 */
/*	    <filestem>.data  (the training data, if the Cubist model	 */
/*			      uses instances as well as rules)		 */
/*									 */
/*	Please note: the names file and data file (if required)		 */
/*	must be exactly as they were when the model was generated.	 */
/*									 */
/*	For each case in <filestem.cases>, the program prints the	 */
/*	target value and then the value predicted by the model.		 */
/*									 */
/*	Revised November 2005						 */
/*									 */
/*************************************************************************/

/* revise to accept binary input by Feng Gao (7/26/2006) */


#include "defns.h"
#include "global.c"
#include "hooks.c"

/*#define DEBUG*/
#ifdef DEBUG
  #define DEBUG_icol 1510
  #define DEBUG_irow 2433
#endif

/*************************************************************************/
/*									 */
/*	Main                                                             */
/*									 */
/*************************************************************************/
extern float	*AttMinD;
extern String	PropVal;


char NameBuff[100];
Description		Case;
RRuleSet		*CubistModel;
FILE		*F;

int InitLAIModel(char *namestem)
{
    int			 m;
    char		Msg[100];
    Boolean		PreviewModel=false;

   
    printf("Loading regression tree");
	strcpy(NameBuff, namestem);
	FileStem = NameBuff;
	PreviewModel = true;

    /*  Read information on attribute names and values  */

    if ( ! (F = GetFile(".names", "r")) ) Error(0, Fn, "");
    GetNames(F);

    /*  Read the model file that defines the ruleset and sets values
	for various global variables such as USEINSTANCES  */

    CubistModel = GetCommittee(".model");

    if ( PreviewModel )
    {
		/*  Display the rulesets  */

		ForEach(m, 0, MEMBERS-1)
		{
			if ( MEMBERS > 1 )
			{
			sprintf(Msg, "Model %d:", m+1);
			}
			else
			{
			sprintf(Msg, "Model:");
			}
			PrintRules(CubistModel[m], Msg);
		}
    }

    if ( USEINSTANCES )
    {
		if ( ! (F = GetFile(".data", "r")) ) Error(0, Fn, "");
		GetData(F, true, false);

		/*  Prepare the file of instances and the kd-tree index  */

		InitialiseInstances(CubistModel);

		free(Item);
		if ( PreviewModel ) printf("\nUsing instances (%d nearest neighbors)"
					   " together with rules\n\n", NN);
    }
    else   if ( PreviewModel )
    {
		printf("\nUsing rules alone\n\n");
    }


    Case = AllocZero(MaxAtt+2, AttValue);
	return 1;

}


float Predict_LAI()
{
    float		Predicted;

	ReplaceUnknowns(Case, Nil);
	Predicted = ( USEINSTANCES ? NNEstimate(CubistModel, Case, &GNNEnv) :PredictValue(CubistModel, Case) );
    return Predicted;
}


void CleanUpLAIModel()
{
    FreeCase(Case);

    /*  Close the case file and free allocated memory  */    
    fclose(F);
    
    FreeCttee(CubistModel);

    if ( USEINSTANCES )
    {
		FreeCases(Instance, MaxInstance);
		FreeUnlessNil(Ref);
		FreeUnlessNil(RSPredVal);
		FreeUnlessNil(GNNEnv.AttMinD);
		FreeIndex(KDTree);
    }

    FreeNamesData();
    FreeUnlessNil(PropVal);
    FreeUnlessNil(IgnoredVals);
}
