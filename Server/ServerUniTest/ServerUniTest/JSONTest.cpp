#include "Common.h"
#include "json\reader.h"
#include "json\json.h"

TEST(JSON, PARSER)
{
	/*std::string jsonStr = "{\
								\"glossary\": {													\
									\"title\": \"example glossary\",							\
									\"GlossDiv\" : {											\
										\"title\": \"S\",										\
										\"GlossList\" : {										\
												\"GlossEntry\": {								\
													\"ID\": \"SGML\",							\
													\"SortAs\" : \"SGML\",						\
													\"GlossTerm\" : \"Standard Generalized Markup Language\",\
													\"Acronym\" : \"SGML\",						\
													\"Abbrev\" : \"ISO 8879:1986\",				\
														\"GlossDef\" : {\
															\"para\": \"A meta-markup language, used to create markup languages such as DocBook.\",\
															\"GlossSeeAlso\" : [\"GML\", \"XML\"]\
														},\
													\"GlossSee\" : \"markup\"\
													}\
												}\
											}\
										}\
							}";

	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(jsonStr, root);
	if (!parsingSuccessful) {
		
	}

	std::string strGlossDiv;
	std::string str = root["glossary"].get("title", 0).asString();;
	
*/

	Json::Value root;
	Json::Value encoding;
	root["encoding"] = "UTF-8";
	Json::Value plugins;
	plugins.append("python");
	plugins.append("c++");
	plugins.append("ruby");
	root["plug-ins"] = plugins;
	Json::Value indent;
	indent["length"] = 3;
	indent["use_space"] = true;
	//root["indent"] = indent;
	root["data"].append(indent);
	root["data"].append(indent);
	root["data"].append(indent);
	//root["encoding"] = "UTF-7";
	Json::StyledWriter writer;
	std::string outputConfig = writer.write(root);
	printf("%s\n", outputConfig.c_str());
	//std::string a;

}