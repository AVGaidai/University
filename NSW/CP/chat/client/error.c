/*
 * Library containing the functions, whitch print error messages
 */
#include <stdio.h>


/*
 * Print error message *mes and returning value -1
 * This function reset background and font color
 */
int print_err (char *mes)
{
    fprintf (stderr, "\E[31mERROR:\E[0m %s\n", mes);

    return -1;
}


/*
 * Print warning message *mes and returning value -1
 * This function reset background and font color
 */
int print_war (char *mes)
{
    fprintf (stdout, "\E[35;1mWARNING:\E[0m %s\n", mes);

    return -2;
}



/*
 * Print info message *mes and returning value -1
 * This function reset background and font color
 */
int print_info (char *mes)
{
    fprintf (stdout, "\E[32;1mINFO:\E[0m %s\n", mes);

    return -2;
}
