void listAll(const Class* c, string path){  // two-parameter overload
	path += c->name();
	cout << path << endl;
	if (c->subclasses().size() == 0) {
		return;
	}
	else {
		path += "=>";
		for (int i = 0; i < c->subclasses().size(); i++) {
			listAll(c->subclasses()[i], path);
		}
		return;
	}
}