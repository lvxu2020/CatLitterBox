结合 KTV 用于数据封装成Json格式后通信而用

实例：write~~~~~~~~~~~
	KTV::Map_t map;
	MJson::Value speedJson
	MJson::FastWriter writer;
	
	speedJson["speed"] = 666；//
	speedJson["speedUnit"] = 1;//
	map["speed"] = KTV::Variant(writer.write(speedJson));

	KTV::Writer::Write(map,sendstr);//Weite函数自己去看

	/***    sendstr字符串结构     ***/
	speed:s:{"speed":666,"speedUnit":0} 

	Map_value(map, "rpm", KTV::Variant(666));//Variant会把数据类型给你识别出来
	KTV::Writer::Write(map,sendstr);
	/***    sendstr字符串结构     ***/
	rpm:n:666 

	/***  map元素多的时候    ***/
	electric:s:{"electric":"33"}

	rpm:n:6
	speed:s:{"speed":7,"speedUnit":0}

	void Map_value(KTV::Map_t& map, const char* mKey, const KTV::Variant& mValue) 
	{
		std::string key = mKey;
    		map.insert(std::make_pair(key, mValue));
	}
	
实例：read~~~~~~~~~~~

	KTV::Map_t data;
	std::string readStr = "electric:s:{"electric":"33"}"
	KTV::Reader::Read(data, readStr);























