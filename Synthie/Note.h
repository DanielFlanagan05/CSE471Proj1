#pragma once
#include <string>
class CNote
{
public:
	CNote();
	virtual ~CNote();
	int Measure() const { return m_measure; }
	double Beat() const { return m_beat; }
	const std::wstring& Instrument() const { return m_instrument; }
	IXMLDOMNode* Node() { return m_node; }
	void XmlLoad(IXMLDOMNode* xml, std::wstring& instrument);
	bool operator<(const CNote& b);

	//getter and setters
	std::wstring GetName() const { return m_name; }
	double GetDuration() const { return m_duration; }
private:
	std::wstring m_instrument;
	int m_measure;
	double m_beat;
	CComPtr<IXMLDOMNode> m_node;

	std::wstring m_name;
	double m_duration;
};

