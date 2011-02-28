// MiracleException.h: interface for the CMiracleException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIRACLEEXCEPTION_H__A9E9BBE1_1B41_11D6_B753_F626AECB4470__INCLUDED_)
#define AFX_MIRACLEEXCEPTION_H__A9E9BBE1_1B41_11D6_B753_F626AECB4470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class MBASEDLL_EXP CMiracleException : public CMiracleObj  
{
public:
	CMiracleException();
	CMiracleException(LPCTSTR reason):CMiracleObj("MiracleException"){m_Reason = reason;m_Reason.Format("MiracleException thrown :%s",reason);logError(m_Reason);};
	CString m_Reason;
	LPCTSTR GetReason() {return m_Reason;}
	virtual ~CMiracleException();

};

#endif // !defined(AFX_MIRACLEEXCEPTION_H__A9E9BBE1_1B41_11D6_B753_F626AECB4470__INCLUDED_)
