package com.oreveins.client;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;

public class XmlUtil {

	public static String toXml(Object obj) {
		XStream xstream = new XStream(new DomDriver("utf-8"));
		xstream.processAnnotations(obj.getClass());
		xstream.autodetectAnnotations(false);
		return xstream.toXML(obj);
	}

	@SuppressWarnings("unchecked")
	public static <T> T toBean(String xmlStr, Class<T> cls) {
		XStream xstream = new XStream(new DomDriver());
		xstream.processAnnotations(cls);
		T obj = (T) xstream.fromXML(xmlStr);
		return obj;
	}
}