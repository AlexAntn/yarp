/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_ROBOTINTERFACE_XMLREADER_H
#define YARP_ROBOTINTERFACE_XMLREADER_H

#include <yarp/robotinterface/api.h>
#include <yarp/robotinterface/experimental/Robot.h>

#include <string>


namespace yarp {
namespace robotinterface {
namespace experimental {

/**
 * Result of the parsing of XMLReader.
 */
class YARP_robotinterface_API XMLReaderResult
{
public:
    static XMLReaderResult ParsingFailed()
    {
        XMLReaderResult result;
        result.parsingIsSuccessful = false;
        return result;
    }

    /**
     * True if the parsing was successful, false otherwise.
     */
    bool parsingIsSuccessful = false;

    /**
     * If parsingIsSuccessful is true, contains a valid robot instance.
     */
    Robot robot;
};

class YARP_robotinterface_API XMLReader
{
public:
    XMLReader();
    virtual ~XMLReader();

    /**
     * Parse the XML description of a robotinterface from a file.
     *
     * \param filename path to the XML file to load.
     * \return result of parsing.
     */
    XMLReaderResult getRobotFromFile(const std::string& filename);

    /**
     * Parse the XML description of a robotinterface from a string.
     *
     * \param xmlString string containing the XML code to parse.
     * \return result of parsing.
     */
    XMLReaderResult getRobotFromString(const std::string& xmlString);
    void setVerbose(bool verbose);
    void setEnableDeprecated(bool enab);

private:
    class Private;
    Private* const mPriv;

}; // class XMLReader

} // namespace experimental
} // namespace robotinterface
} // namespace yarp

#endif // YARP_ROBOTINTERFACE_XMLREADER_H
