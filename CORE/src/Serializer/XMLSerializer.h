#pragma once

#include <Maths/Maths.h>
#include <TinyXML/tinyxml2.h>

using namespace tinyxml2;

namespace Orbit::XML {

	class Reader {
	public:
		FORCEINLINE Reader() = default;
		FORCEINLINE ~Reader() {
			_Document.Clear();
		}

		FORCEINLINE void Close() {
			_Document.Clear();
		}

		FORCEINLINE void Open(const char* filename) {		
			_Document.LoadFile(filename);
			if (_Document.Error()) {
				OB_ERROR("failed to open xml: [%s] %s", filename, _Document.ErrorStr());
				return;
			}
		}

		FORCEINLINE tinyxml2::XMLElement* Root() {
			return _Document.RootElement();			
		}

		FORCEINLINE std::vector<XMLElement*> ChildElements(XMLElement* el = nullptr) {
			std::vector<tinyxml2::XMLElement*> children;

			if (el == nullptr) {
				for (tinyxml2::XMLElement* e = _Document.FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
					children.push_back(e);
				}
			}
			else {
				for (tinyxml2::XMLElement* e = el->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
					children.push_back(e);
				}
			}
			return children;
		}

		// get int from xml
		FORCEINLINE int GetInt(tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			return e->IntAttribute("value");
		}

		// get int from xml
		FORCEINLINE int GetIntAttr(const char* name, tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			return e->IntAttribute(name);
		}

		// get float from xml
		FORCEINLINE float GetFloat(tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			return e->FloatAttribute("value");
		}

		// get float from xml
		FORCEINLINE float GetFloatAttr(const char* name, tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			return e->FloatAttribute(name);
		}

		// get string from xml
		FORCEINLINE const std::string& GetString(tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			return e->Attribute("value");
		}

		// get string from xml
		FORCEINLINE const std::string& GetStringAttr(const char* name, tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			return e->Attribute(name);
		}

		// get Vec3 from xml
		FORCEINLINE const glm::vec3& GetVec3(tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			float x = e->FloatAttribute("x");
			float y = e->FloatAttribute("y");
			float z = e->FloatAttribute("z");
			return vec3f(x, y, z);
		}

		// get color4 from xml
		FORCEINLINE const glm::vec3& GetColor3(tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			float r = e->FloatAttribute("r");
			float g = e->FloatAttribute("g");
			float b = e->FloatAttribute("b");
			return vec3f(r, g, b);
		}

		// get color4 from xml
		FORCEINLINE const glm::vec4& GetColor4(tinyxml2::XMLElement* e) {
			ASSERT(e && "Invalid XML-Element!");
			float r = e->FloatAttribute("r");
			float g = e->FloatAttribute("g");
			float b = e->FloatAttribute("b");
			float a = e->FloatAttribute("a");
			return vec4f(r, g, b, a);			
		}

	private:
		tinyxml2::XMLDocument _Document;
	};

	class Writer {
	public:
		FORCEINLINE Writer(): _PushedTagCount(0), _FILE(nullptr), _Printer(nullptr) {}
		FORCEINLINE ~Writer() { 
			OB_INFO("XML::Writer destroyed!");
			OB_SAFE_DELETE(_Printer);
		}

		FORCEINLINE void Open(const char* filename) {
			_FILE = fopen(filename, "w+");
			_Printer = new tinyxml2::XMLPrinter(_FILE);
			_Printer->PushHeader(true, true);
		}

		FORCEINLINE void Close(bool compact = false) {
			tinyxml2::XMLDocument document;
			document.Parse(_Printer->CStr());	
			
			if (_FILE != nullptr) { 
				fclose(_FILE); 
				_Printer->ClearBuffer();
			}
			ASSERT(_PushedTagCount == 0 && "XML-Tag not closed!");
		}

		FORCEINLINE void PushTag(const char* name) {		
			_Printer->OpenElement(name);
			++_PushedTagCount;
		}

		FORCEINLINE void PopTag() {
			ASSERT(_PushedTagCount > 0 && "XML-Tag not closed!");
			_Printer->CloseElement();
			--_PushedTagCount;
		}


		// serialize int
		FORCEINLINE void PushIntAttr(const char* name, int value) {			
			_Printer->PushAttribute(name, value);
		}

		FORCEINLINE void PushFloatAttr(const char* name, float value) {
			_Printer->PushAttribute(name, value);
		}

		FORCEINLINE void PushStringAttr(const char* name, const char* value) {			
			_Printer->PushAttribute(name, value);
		}

		// ------------------------------------------------------
		
		// serialize int
		FORCEINLINE void PushInt(const char* tagName, int value) {
			_Printer->OpenElement(tagName);
			_Printer->PushAttribute("value", value);
			_Printer->CloseElement(true);
		}

		// serialize int
		FORCEINLINE void PushFloat(const char* tagName, float value) {
			_Printer->OpenElement(tagName);
			_Printer->PushAttribute("value", value);
			_Printer->CloseElement(true);
		}

		// serialize int
		FORCEINLINE void PushString(const char* tagName, const char* value) {
			_Printer->OpenElement(tagName);
			_Printer->PushAttribute("value", value);
			_Printer->CloseElement(true);
		}

		// serialize vector3
		FORCEINLINE void PushVec3(const char* tagName, const vec3f& v) {
			_Printer->OpenElement(tagName);
			_Printer->PushAttribute("x", v.x);
			_Printer->PushAttribute("y", v.y);
			_Printer->PushAttribute("z", v.z);
			_Printer->CloseElement(true);
		}

		// serialize vector3
		FORCEINLINE void PushColor3(const char* tagName, const vec3f& c) {
			_Printer->OpenElement(tagName);
			_Printer->PushAttribute("r", c.r);
			_Printer->PushAttribute("g", c.g);
			_Printer->PushAttribute("b", c.b);
			_Printer->CloseElement(true);
		}

		// serialize color4
		FORCEINLINE void PushColor4(const char* tagName, const vec4f& c) {
			_Printer->OpenElement(tagName);
			_Printer->PushAttribute("r", c.r);
			_Printer->PushAttribute("g", c.g);
			_Printer->PushAttribute("b", c.b);
			_Printer->PushAttribute("a", c.a);
			_Printer->CloseElement(true);
		}
		
	private:	
		FILE* _FILE;
		int32 _PushedTagCount;			
		tinyxml2::XMLPrinter* _Printer;
	};
}