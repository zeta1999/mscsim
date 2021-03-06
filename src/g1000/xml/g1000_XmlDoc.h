/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
#ifndef G1000_XMLDOC_H
#define G1000_XMLDOC_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <libxml/tree.h>

#include <g1000/xml/g1000_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief XML Document class.
 */
class XmlDoc
{
public:

    /** Constrcutor. */
    XmlDoc( const std::string &fileName = std::string() );

    /** Destrcutor. */
    virtual ~XmlDoc();

    /** */
    inline XmlNode getRootNode()
    {
        return XmlNode( *_root );
    }

    /** */
    inline bool isOpen() const
    {
        return _open;
    }

    /** @return true on success, false on failure. */
    bool readFile( const std::string &fileName );

private:

    xmlDocPtr _doc;
    bool _open;
    XmlNode *_root;
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_XMLDOC_H
