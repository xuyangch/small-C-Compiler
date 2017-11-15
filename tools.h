/*************************************************************************
                        CHECK VALID

This file contains checkValid function
***************************************************************************/
//checkValid is used to check whether the string s is a reserved word.
//Return 1 if not reserved, 0 if reserved
int checkValid(char * s)
{
	if ((strcmp(s, "int") == 0) || (strcmp(s, "struct") == 0) || (strcmp(s, "return") == 0) || (strcmp(s, "if") == 0)
		|| (strcmp(s, "else") == 0) || (strcmp(s, "break") == 0) || (strcmp(s, "cont") == 0) || (strcmp(s, "for") == 0)
		|| (strcmp(s, "read") == 0) || (strcmp(s, "write") == 0)
		)
	{
		return 0;
	}
	return 1;
}