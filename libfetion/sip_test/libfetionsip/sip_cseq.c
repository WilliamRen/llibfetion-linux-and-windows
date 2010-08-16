
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sip_port.h"
#include "sip_message.h"

//#include "sip_cseq.h"

int
osip_cseq_init (osip_cseq_t ** cseq)
{
  *cseq = (osip_cseq_t *) osip_malloc (sizeof (osip_cseq_t));
  if (*cseq == NULL)
    return OSIP_NOMEM;
  (*cseq)->method = NULL;
  (*cseq)->number = NULL;
  return OSIP_SUCCESS;
}


/* fills the cseq header of message.                        */
/* INPUT :  char *hvalue | value of header.                 */
/* OUTPUT: osip_message_t *sip | structure to save results. */
/* returns -1 on error.                                     */

int
osip_message_set_cseq (osip_message_t * sip, const char *hvalue)
{
  int i;

  if (hvalue == NULL || hvalue[0] == '\0')
    return OSIP_SUCCESS;

  if (sip->cseq != NULL)
    return OSIP_BADPARAMETER;
  i = osip_cseq_init (&(sip->cseq));
  if (i != 0)
    return i;
  sip->message_property = 2;
  i = osip_cseq_parse (sip->cseq, hvalue);
  if (i != 0)
    {
      osip_cseq_free (sip->cseq);
      sip->cseq = NULL;
      return i;
    }
  return OSIP_SUCCESS;
}

/* fills the cseq strucuture.                               */
/* INPUT : char *hvalue | value of header.                  */
/* OUTPUT: osip_message_t *sip | structure to save results. */
/* returns -1 on error.                                     */
int
osip_cseq_parse (osip_cseq_t * cseq, const char *hvalue)
{
  char *method = NULL;
  const char *end = NULL;

  if (cseq==NULL || hvalue==NULL)
	  return OSIP_BADPARAMETER;

  cseq->number = NULL;
  cseq->method = NULL;

  method = strchr (hvalue, ' ');        /* SEARCH FOR SPACE */
  if (method == NULL)
    return OSIP_SYNTAXERROR;

  end = hvalue + strlen (hvalue);

  if (method - hvalue + 1 < 2)
    return OSIP_SYNTAXERROR;
  cseq->number = (char *) osip_malloc (method - hvalue + 1);
  if (cseq->number == NULL)
    return OSIP_NOMEM;
  osip_clrncpy (cseq->number, hvalue, method - hvalue);

  if (end - method + 1 < 2)
    return OSIP_SYNTAXERROR;
  cseq->method = (char *) osip_malloc (end - method + 1);
  if (cseq->method == NULL)
    return OSIP_NOMEM;
  osip_clrncpy (cseq->method, method + 1, end - method);

  return OSIP_SUCCESS;                     /* ok */
}

/* returns the cseq header.                     */
/* INPUT : osip_message_t *sip | sip message.   */
/* returns null on error.                       */
/*
osip_cseq_t *
osip_message_get_cseq (const osip_message_t * sip)
{
  return sip->cseq;
}*/

char *
osip_cseq_get_number (osip_cseq_t * cseq)
{
  return cseq->number;
}

char *
osip_cseq_get_method (osip_cseq_t * cseq)
{
  return cseq->method;
}

void
osip_cseq_set_number (osip_cseq_t * cseq, char *number)
{
  cseq->number = (char *) number;
}

void
osip_cseq_set_method (osip_cseq_t * cseq, char *method)
{
  cseq->method = (char *) method;
}

/* returns the cseq header as a string.          */
/* INPUT : osip_cseq_t *cseq | cseq header.      */
/* returns null on error.                        */
int
osip_cseq_to_str (const osip_cseq_t * cseq, char **dest)
{
  size_t len;

  *dest = NULL;
  if ((cseq == NULL) || (cseq->number == NULL) || (cseq->method == NULL))
    return OSIP_BADPARAMETER;
  len = strlen (cseq->method) + strlen (cseq->number) + 2;
  *dest = (char *) osip_malloc (len);
  if (*dest == NULL)
    return OSIP_NOMEM;
  sprintf (*dest, "%s %s", cseq->number, cseq->method);
  return OSIP_SUCCESS;
}

/* deallocates a osip_cseq_t structure.  */
/* INPUT : osip_cseq_t *cseq | cseq. */
void
osip_cseq_free (osip_cseq_t * cseq)
{
  if (cseq == NULL)
    return;
  osip_free (cseq->method);
  osip_free (cseq->number);
  osip_free (cseq);
}

int
osip_cseq_clone (const osip_cseq_t * cseq, osip_cseq_t ** dest)
{
  int i;
  osip_cseq_t *cs;

  *dest = NULL;
  if (cseq == NULL)
    return OSIP_BADPARAMETER;
  if (cseq->method == NULL)
    return OSIP_BADPARAMETER;
  if (cseq->number == NULL)
    return OSIP_BADPARAMETER;

  i = osip_cseq_init (&cs);
  if (i != 0)
    {
      osip_cseq_free (cs);
      return i;
    }
  cs->method = osip_strdup (cseq->method);
  cs->number = osip_strdup (cseq->number);

  *dest = cs;
  return OSIP_SUCCESS;
}

int
osip_cseq_match (osip_cseq_t * cseq1, osip_cseq_t * cseq2)
{
  if (cseq1 == NULL || cseq2 == NULL)
    return OSIP_BADPARAMETER;
  if (cseq1->number == NULL || cseq2->number == NULL
      || cseq1->method == NULL || cseq2->method == NULL)
    return OSIP_BADPARAMETER;

  if (0 == strcmp (cseq1->number, cseq2->number))
    {
      if (0 == strcmp (cseq2->method, "INVITE")
          || 0 == strcmp (cseq2->method, "ACK"))
        {
          if (0 == strcmp (cseq1->method, "INVITE") ||
              0 == strcmp (cseq1->method, "ACK"))
            return OSIP_SUCCESS;
      } else
        {
          if (0 == strcmp (cseq1->method, cseq2->method))
            return OSIP_SUCCESS;
        }
    }
  return OSIP_UNDEFINED_ERROR;
}
