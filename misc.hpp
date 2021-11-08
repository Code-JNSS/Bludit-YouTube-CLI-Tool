struct isSpace
{
	bool operator()(unsigned c)
	{
		return (c == ' ' || c == '\n' || c == '\r' ||
				c == '\t' || c == '\v' || c == '\f');
	}
};


// To escape input with HTML special chars
void escape(string *data)
{
    string::size_type pos = 0;
    for (;;)
    {
        pos = data->find_first_of("\"&<>", pos);
        if (pos == string::npos) break;
        std::string replacement;
        switch ((*data)[pos])
        {
        case '\"': replacement = "&quot;"; break;   
        case '&':  replacement = "&amp;";  break;   
        case '<':  replacement = "&lt;";   break;   
        case '>':  replacement = "&gt;";   break;          
        default: ;
        }
        data->replace(pos, 1, replacement);
        pos += replacement.size();
    };
}

// Exec a command (in Windows environnements)
string exec(string command) {
   char buffer[128];
   string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}