import sys
import getopt
import time

if __name__ == '__main__':
    if len(sys.argv) != 7:
        print("usage:\n\n gen_flash_xml.py cfg board da.bin scatter.xml build_version ./<flash.xml> \n")
        print(sys.argv)
        sys.exit(-1)

    opts, args = getopt.getopt(sys.argv[1:], '')

    try:
        build_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        cfg = args[0].split(",")
        flash_str = ("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
                    "<flash-mode>\n"
                    "    <project>{0}</project>\n"
                    "    <dagent>{1}</dagent>\n"
                    "    <scatter>../{2}</scatter>\n"
                    "    <build_version>{3}</build_version>\n"
                    "    <build_time>{4}</build_time>\n").format(args[1], args[2], args[3], args[4], build_time)

        for item in cfg:
            pair = item.split("=")
            flash_str += "    <" + pair[0] + ">" + pair[1] +"</" + pair[0] + ">\n"
        flash_str += "</flash-mode>\n"

        flash_xml_bin = open(args[5]+"/flash.xml", "w+b")
        flash_xml_bin.write(flash_str.encode('utf-8'))
        flash_xml_bin.close()
        
        flash_xsd = ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                    "<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\">\n"
                    "<xs:element name=\"flash-mode\" type=\"flashInfo\"/>\n"
                    "  <xs:complexType name=\"flashInfo\">\n"
                    "    <xs:sequence>\n"
                    "      <xs:element name=\"project\" type=\"xs:string\"/>\n"
                    "      <xs:element name=\"dagent\" type=\"xs:string\"/>\n"
                    "      <xs:element name=\"scatter\" type=\"xs:string\"/>\n"
                    "      <xs:element name=\"build_version\" type=\"xs:string\"/>\n"
                    "      <xs:element name=\"build_time\" type=\"xs:string\"/>\n")

        for item in cfg:
            pair = item.split("=")
            flash_xsd += "      <xs:element name=\"" + pair[0] + "\" type=\"xs:string\"/>\n"

        flash_xsd += ("    </xs:sequence>\n"
                     "  </xs:complexType>\n"
                     "</xs:schema>\n")
        flash_xsd_bin = open(args[5]+"/flash.xsd", "w+b")
        flash_xsd_bin.write(flash_xsd.encode('utf-8'))
        flash_xsd_bin.close()
    except Exception as e:
        print("gen_flash_xml.py exception:")
        print(str(e))
        sys.exit(-1)
    finally:
        sys.exit(0)

