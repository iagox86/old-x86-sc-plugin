
#include "Match.h"

#include <ctype.h>

/* I believe I got this implementation from Skywing. */
int match(const char *mask, const char *name)
{
    int wild=0, q=0;
    const char *m=mask, *n=name, *ma=mask, *na=name;

    for(;;)
    {
        if (*m == '*')
        {
        while (*m == '*') ++m;
             wild = 1;
             ma = m;
        na = n;
        }


        if (!*m)
        {
            if (!*n)
                return 1;

            for (--m; (m > mask) && (*m == '?'); --m) ;

            if ((*m == '*') && (m > mask) && (m[-1] != '\\'))  
                return 1;
            if (!wild)
                return 0;
    
            m = ma;
        }
        else if (!*n)
        { 
             while(*m == '*') ++m;
             return (*m == 0);
        } 
        
        if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?')))
        {
            ++m; 
            q = 1;
        }  
        else
        {
            q = 0; 
        }

        if ((tolower(*m) != tolower(*n)) && ((*m != '?') || q))
        {
             if (!wild) return 0;
             m = ma;
             n = ++na;
        }
        else
        {
             if (*m) ++m;
             if (*n) ++n;
        }
    }
}


static char *RemoveSingleChar(char *String, size_t Location)
{
	size_t i;

	/* If it's going to overflow, the best way can do is make no changes. */
	if(Location >= strlen(String))
		return String;

    for(i = Location; String[i] != '\0'; i++)
        String[i] = String[i + 1];

    return String;
}

static char *InsertSingleChar(char *String, size_t Location, char Char, size_t maxlen)
{
    size_t i;

    char Temp = String[Location];
    char OldTemp;

	/* If it's going to overflow, the best way can do is make no changes. */
	if(Location >= maxlen || strlen(String) + 1 >= maxlen)
		return String;

    String[Location] = Char;



    for(i = Location + 1; Temp != '\0'; i++)
    {
        OldTemp = Temp;
        Temp = String[i];
        String[i] = OldTemp;
    }

    String[i] = '\0';

    return String;
}

BOOL match_replace(const char *Find, const char *Replace, char *String, size_t maxlen)
{
    int i = 0;
    int j = 0;

    char *FindWild = (char *) malloc((strlen(Find) + 2) * sizeof(char));

	BOOL replaced = FALSE;

    while(Find[i] != '\0')
    {
        if(Find[i] == '*')
            FindWild[i] = '?';
        else
            FindWild[i] = Find[i];

        i++;
    }

    FindWild[i] = '*';
    FindWild[i + 1] = '\0';
    // Reset our loop variable
    i = 0;

    while(String[i] != '\0' && !replaced)
    {
        // While we aren't done String
        if(match(FindWild, String + i))
        {
            // Great, we have a match!

            // Remove find characters from string
            j = 0;
            while(Find[j] != '\0')
            {
                RemoveSingleChar(String, i);
                j++;
            }

            j = 0;
            while(Replace[j] != '\0')
            {
                // If for some reason you want to insert the string backwards, just remove
                // "+ j" from the next line :D

                InsertSingleChar(String, i + j, Replace[j], maxlen);
                j++;
            }
            replaced = TRUE;
        }
    // can't forget to increment!
    i++;
    }

	free(FindWild);

    return replaced;
}

BOOL match_replaceall(const char *Find, const char *ReplaceChar, char *String, size_t maxlen)
{
	/* Using an incrementor to avoid an infinite loop. */
	int i = 0; 
    if(match_replace(Find, ReplaceChar, String, maxlen))
    {
        while(match_replace(Find, ReplaceChar, String, maxlen) && i < 10000)
			i++;
        return TRUE;
    }

    return FALSE;
}

