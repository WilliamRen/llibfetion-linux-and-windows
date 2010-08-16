#ifndef SIP_CSEQ_H_INCLUDED
#define SIP_CSEQ_H_INCLUDED

/**
 * @file osip_cseq.h
 * @brief oSIP osip_cseq header definition.
 */

/**
 * @defgroup oSIP_CSEQ oSIP cseq header definition.
 * @ingroup oSIP_HEADERS
 * @{
 */

/**
 * Structure for CSeq headers.
 * @var osip_cseq_t
 */
  typedef struct osip_cseq osip_cseq_t;

/**
 * Definition of the CSeq header.
 * @struct osip_cseq
 */
  struct osip_cseq
  {
    char *method;    /**< CSeq method */
    char *number;    /**< CSeq number */
  };

#ifdef __cplusplus
extern "C"
{
#endif

  int osip_cseq_init (osip_cseq_t ** header);

  void osip_cseq_free (osip_cseq_t * header);

  int osip_cseq_parse (osip_cseq_t * header, const char *hvalue);

  int osip_cseq_to_str (const osip_cseq_t * header, char **dest);

  int osip_cseq_clone (const osip_cseq_t * header, osip_cseq_t ** dest);

  void osip_cseq_set_number (osip_cseq_t * header, char *value);

  char *osip_cseq_get_number (osip_cseq_t * header);

  void osip_cseq_set_method (osip_cseq_t * header, char *value);

  char *osip_cseq_get_method (osip_cseq_t * header);

  int osip_cseq_match (osip_cseq_t * cseq1, osip_cseq_t * cseq2);

  //int osip_message_set_cseq (osip_message_t * sip, const char *hvalue);

#ifdef __cplusplus
}
#endif


#endif // SIP_CSEQ_H_INCLUDED
