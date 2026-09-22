#include "XMLUtils.hpp"

jpl::_parser::_xml::RootElement* jpl::_parser::_xml::parse(std::string buffer){
    //Parsing header
    size_t end = jpl::_utils::_string::getIndexGroupOver(buffer, std::regex("<"), std::regex(">"));
    std::string header = buffer.substr(0, end+1);
    if(header.find("<?xml ") != 0){
        throw jpl::_parser::_xml::_exception::XMLParsingException("XML Header must begin with '<?xml '");
    }
    if(header.find_last_of("?>") != header.size()-1){//-1 due to ?
        throw jpl::_parser::_xml::_exception::XMLParsingException("XML Header must end with '?>'");
    }
    buffer = jpl::_utils::_string::trim(buffer.substr(header.size()));  //Clean buffer from header
    header = header.substr(5, header.size() - 5 - 2);   //Remove '<?xml' and '?>'
    header = jpl::_utils::_string::trim(header);
    std::string version = "1.0", encoding = "UTF-8";
    parseHeaderAttribute(header, "version", version);
    try{    //Check if version is valid
        std::atof(version.c_str());
    }catch(std::exception* ex){
        throw jpl::_parser::_xml::_exception::XMLParsingException("Version " + version + " is not valid");
    }
    parseHeaderAttribute(header, "encoding", encoding);

    RootElement* root = new RootElement("unnamed", version, encoding);
    jpl::_parser::_xml::parseElement(buffer, root, 0);
    return root;
}

static void jpl::_parser::_xml::parseHeaderAttribute(std::string &buffer, std::string nameAttribute, std::string &attributeOut){
    if(jpl::_utils::_string::startWith(buffer, std::regex(nameAttribute + "="))){    //Version is not mandatory
        if(buffer.at(nameAttribute.size()+1) != '\"'){
            throw jpl::_parser::_xml::_exception::XMLParsingException("Attribute must be enclosed by double-quote");
        }
        buffer = buffer.substr(nameAttribute.size()+2);
        size_t endAttribute = buffer.find('\"');
        if(endAttribute == std::string::npos)   //Closed quote missing
            throw jpl::_parser::_xml::_exception::XMLParsingException("Attribute must be enclosed by double-quote");
        attributeOut = buffer.substr(0, endAttribute);
        buffer = buffer.substr(endAttribute+1);
        buffer = jpl::_utils::_string::trim(buffer);
    }
}

static void jpl::_parser::_xml::parseElement(std::string &buffer, Element* element, unsigned short tabs){
    size_t endRootHeader = jpl::_utils::_string::getIndexGroupOver(buffer, std::regex("<"), std::regex(">"));
    if(endRootHeader == std::string::npos)
        throw jpl::_parser::_xml::_exception::XMLParsingException("XML Element declaration is not valid");
    std::string tmp = jpl::_utils::_string::trim(buffer.substr(1, endRootHeader));
    bool shortQuit = tmp.at(tmp.size()-2) == '/';
    size_t endNameRoot = tmp.find(" ");
    if(endNameRoot == std::string::npos)
        endNameRoot = endRootHeader-1;//If there are no empty space, it may be that there are no attributes
    std::string rootName = tmp.substr(0, endNameRoot - (shortQuit ? 1 : 0) );
    tmp = tmp.substr(endNameRoot, tmp.size() - 1 - endNameRoot);
    buffer = buffer.substr(endRootHeader+1);//remove header of root
    validateName(rootName);
    element->setName(rootName);
    parseAttributes(tmp, element);
    if(shortQuit)
        return;
    while(!buffer.empty()){
        buffer = jpl::_utils::_string::trim(buffer);
        if(buffer.at(0) != '<'){    //There's some text
            size_t endText = buffer.find("<");
            if(endText == std::string::npos)
                throw jpl::_parser::_xml::_exception::XMLParsingException("Could not find '<' in order to finish text-parsing of " + rootName + " element ");
            std::string text = jpl::_utils::_string::endTrim(buffer.substr(0, endText));
            element->setText(text);
            buffer.erase(0, endText);
        }
        if(buffer.find("</") == 0){ //The previous element is over
            buffer.erase(0, 2);
            if(buffer.at(0) == ' ')
                throw jpl::_parser::_xml::_exception::XMLParsingException("The close tag of an element does not have to contains empty space");
            if(buffer.find(rootName + ">") == 0){
                buffer.erase(0, rootName.size()+1);
                buffer = jpl::_utils::_string::startTrim(buffer);
            }else{
                throw jpl::_parser::_xml::_exception::XMLParsingException("The close tag '</" + rootName + "> has not been found");
            }
            break;
        }
        Element* child = new Element("unnamed");
        child->setTabs(tabs+1);
        parseElement(buffer, child, tabs+1);
        element->addElement(child);
    }

}

static void jpl::_parser::_xml::parseAttributes(std::string buffer, Element* element){
    buffer = jpl::_utils::_string::trim(buffer);
    while(!buffer.empty()){
        size_t eq = buffer.find('=');
        if(eq == std::string::npos)
            throw jpl::_parser::_xml::_exception::XMLParsingException("It seems like an equal sign is missing: " + buffer);
        std::string nameAttribute = buffer.substr(0, eq);
        validateName(nameAttribute);
        buffer = buffer.substr(eq+1);
        char delimiter = buffer.at(0);
        if(delimiter == '\'' || delimiter == '\"'){
            size_t end = buffer.find(delimiter, 1);
            if(end == std::string::npos)
                throw jpl::_parser::_xml::_exception::XMLParsingException("Attribute " + nameAttribute + " is not closed by " + delimiter);
            std::string value = buffer.substr(1, end-1);
            jpl::_parser::_xml::Attribute* attribute = new jpl::_parser::_xml::Attribute(nameAttribute, value);
            element->addAttribute(attribute);
            buffer = buffer.substr(end+1);
            buffer = jpl::_utils::_string::startTrim(buffer);
        }else{
            throw jpl::_parser::_xml::_exception::XMLParsingException("Attribute value must be enclosed by double-quote. Element: " + element->getName() + ". Attribute: " + nameAttribute);
        }
    }
}

void jpl::_parser::_xml::validateName(std::string name){
    if(!jpl::_utils::_string::match(name, std::regex("^[A-Za-z0-9_.-]+$", std::regex::extended))){
        throw jpl::_parser::_xml::_exception::XMLValidationException("Name must contains only letters, digits, hyphens, underscores and periods: " + name);
    }
}