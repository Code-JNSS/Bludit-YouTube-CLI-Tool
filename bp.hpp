#include <iostream>
#include <fstream>
#include <sstream>

/******************************************************************************************
string BP_ChooseConfigFile()
Returns choosen config file number, after prompting the user with the .txt files available
*******************************************************************************************/
string BP_ChooseConfigFile()
{
    /*****************************
    / Choosing file
    /****************************/
	string dir_cfg_cmd = "dir /B config*.txt";
    string dir_cfg_list;
	string cfg_num;
	string confirm_flag;
    
	//Show configX.txt files
	dir_cfg_list = exec(dir_cfg_cmd);
	cout << dye::aqua(dir_cfg_list) << endl;
	
    
    confirm_flag = "no";
    
    while(confirm_flag != "y")
    {
        cout <<  "Enter config file number (eg. 1 for config1.txt) please : " << endl;
        cin >> cfg_num;

        
        string cfg_file;
        string usr_Copyright;
        
        cfg_file = "config";
        cfg_file.append(cfg_num);
        cfg_file.append(".txt");
        
        ifstream cfgReader(cfg_file);
        if(!cfgReader)
        {
            cout << "ERROR: Unable to open " << cfg_file << "!" << endl;
            usr_Copyright = "ERROR!";
        }        
        else
        {
            getline(cfgReader, usr_Copyright);
        }
        
        
        cout << "Confirm config file choice : ";
        cout << usr_Copyright << endl;
        cout << "y/n : ";
        cin >> confirm_flag;
    }
    
    return cfg_num;
}



/******************************************************************************************
void BP_ReadConfigFile(string cfg_num,string data_array[])
Populate data_array with data read from "config" + cfg_num + ".txt"
*******************************************************************************************/
void BP_ReadConfigFile(string cfg_num,string data_array[])
{
    string str_confirm_cat;
    
    string cfg_file;
    string dir_img_cmd;

	string dir_img_list;

    cfg_file = "config";
    cfg_file.append(cfg_num);
    cfg_file.append(".txt");

	ifstream cfgReader(cfg_file);
	if(!cfgReader)
	{
		cout << "ERROR: Unable to open " << cfg_file << "!" << endl;
		return;
	}

    string usr_videoDivClass;
    string usr_Caption;
        
    string usr_Token;
	string usr_Auth;
	string usr_PageAPI;
	string usr_PageCats;
	
    string ht_UsrName;
	string ht_Pwd;
	
    
    //Name of the config file, we dismiss this line
	getline(cfgReader, usr_videoDivClass);
    
	//CSS class for videos alone
	getline(cfgReader, usr_videoDivClass);
    
    //CSS class for videos mixed with pictures
	getline(cfgReader, usr_Caption);
    
	//Bludit token
	getline(cfgReader, usr_Token);
	
	//Bludit auth
	getline(cfgReader, usr_Auth);
	
	//API URL https://mydomain.com/web/api/pages) 
	getline(cfgReader, usr_PageAPI);
	
	//Categories, comma-separated
	getline(cfgReader, usr_PageCats);

    //.htaccess username
	getline(cfgReader, ht_UsrName);

    
    //.htaccess password
	getline(cfgReader, ht_Pwd);

	cfgReader.close();
    
    data_array[0]  = usr_videoDivClass;
    data_array[1]  = usr_Caption;
    data_array[2]  = usr_Token;
    data_array[3]  = usr_Auth;
    data_array[4]  = usr_PageAPI;
    data_array[5]  = usr_PageCats;
    data_array[6]  = ht_UsrName; 
    data_array[7]  = ht_Pwd;
    

    return;
}



/******************************************************************************************
string BP_ChooseCategory(string read_categories)
Splits categories read from config file, prompts user to choose one or none,
Returns the chosen category name
*******************************************************************************************/
string BP_ChooseCategory(string read_categories)
{
    int cpt_cats;
	cpt_cats = 0;
	vector<string> bludit_cats;
    
    string str_confirm_cat;
    
	string delim = ",";
	size_t pos2 = 0;
    string token;
        
	cout << "0. No category" << endl;
	
	while ((pos2 = read_categories.find(delim)) != string::npos)
    {
		cpt_cats++;
		token = read_categories.substr(0,pos2);
		cout << cpt_cats << ". " << token << endl;
		bludit_cats.push_back(token);
		read_categories.erase(0,pos2 + delim.length());
	}
	cpt_cats++;
	cout << cpt_cats << ". " << read_categories << endl;
	bludit_cats.push_back(read_categories);
			

	if(cpt_cats > 0)
	{
		cout <<  "Enter page category please (0 for none) :" << endl;
		cin >> cpt_cats;
		
		cout << "Confirm category : ";
		if(cpt_cats == 0)
				 cout << "no category" << endl;
			else
				cout << bludit_cats[cpt_cats-1] << endl;
		cout << "y/n : ";
		cin >> str_confirm_cat;
			
		while(str_confirm_cat != "y")
		{
			cout <<  "Enter page category please (0 for none) :" << endl;
			cin >> cpt_cats;
		
			cout << "Confirm category : ";
			
			if(cpt_cats == 0)
				 cout << "no category" << endl;
			else
				cout << bludit_cats[cpt_cats-1] << endl;
			
			cout << "y/n : ";
			cin >> str_confirm_cat;
		}
        
        if(cpt_cats == 0)
            return "";
        else
            return bludit_cats[cpt_cats-1];
        
	}
    else
        return "";
}
    



/*********************************************************
string BP_ArticleTags()
Asks user to input page tags, comma-separated.
Returns the string.
**********************************************************/
string BP_ArticleTags()
{
    string my_tags;
    string confirm_tags;
   
    confirm_tags = "n";
   
    while(confirm_tags != "y")
    {        
        cout <<  "Enter page tags, comma separated ('none' for none) :" << endl;
        cin >> my_tags;
        
        if(my_tags == "none")
                 cout << endl << "none" << endl;
            else
                cout << endl << my_tags << endl;
        
        cout << "Confirm (y/n) ?" << endl;
        cout << "y/n : ";
        cin >> confirm_tags; 
    }
    
    return my_tags;
}


/*********************************************************
string BP_RunCurlJSON(string usr_PageAPI)
Runs the CURL command, JSON file formatted in main()
Needs usr_PageAPI, string to the Bludit API end-point (https://mydomain.com/web/api/pages)
Returns article key (string).
**********************************************************/
string BP_RunCurlJSON(string usr_PageAPI, string usr_HT_Name, string usr_HT_Passwd)
{    
	string curl_command;
	string curl_command_result;
	curl_command = "curl -X POST -H \"Content-Type: application/json\" -d @data.json \"";
	curl_command.append(usr_PageAPI);
	curl_command.append("\"");	

    string ht_user = usr_HT_Name;
    string ht_pass = usr_HT_Passwd;
    

    if(ht_user != "")
    {
        curl_command.append(" -u \"");
        curl_command.append(ht_user);                   
        curl_command.append(":");                           
        curl_command.append(ht_pass);
        curl_command.append("\"");
    }

    
	cout << "Running API call with CURL..." << endl;
	curl_command_result = exec(curl_command.c_str());
		
	cout << "Here the summary :" << endl;
	cout << curl_command_result << endl << endl;

	cout << "Key for the article is : " << endl;
	
	string temp_key;
	temp_key = curl_command_result.substr(0,curl_command_result.length() - 3);
	size_t pos = temp_key.find("key");
	cout << temp_key.substr(pos+6,temp_key.length()) << endl << endl;
	temp_key = temp_key.substr(pos+6,temp_key.length());
    
    return temp_key;
	
}


/*********************************************************
void BP_UploadIndexTXT(string cfg_num,string img_dir_name)   
Uploads index.txt in bludit/pages/TEMP_KEY/
It adds our contents to the created page.
Needs cfg_num, config file number
and temp_key, key to our new page
Needs .\winscp\mv_index_script2[config file number].txt
**********************************************************/
void BP_UploadIndexTXT(string cfg_num,string temp_key)    
{     
	string ftp_cmd2;
	ftp_cmd2 = "Winscp.com /script=.\\winscp\\mv_index_script";
	ftp_cmd2.append(cfg_num);
    ftp_cmd2.append(".txt /parameter");    
    ftp_cmd2.append(" ");
	ftp_cmd2.append(temp_key);
	
	cout << "You must have configured mv_index_script" << cfg_num << ".txt." << endl;
	cout << "Running FTP index.txt file moving..." << endl;
	system(ftp_cmd2.c_str());

}

/***********************************************************
void BP_DelDir(string img_dir_name)
Erase img_dir_name folder
***********************************************************/
void BP_DelDir(string img_dir_name)
{
  	string erase_dir_command;
	erase_dir_command = "RMDIR /Q/S "; 
	erase_dir_command.append(img_dir_name);
	system(erase_dir_command.c_str());  
}


/****************************************************************
string BP_HTMLformat(string str_input)
Returns the input string formatted with HTML special chars
****************************************************************/
string BP_HTMLformat(string str_input)
{    
    string my_input = str_input;

    char arr[my_input.length() + 1]; 
 
    string my_html_output;
    my_html_output = "";
    
    strcpy(arr, my_input.c_str()); 
    
    for (int i = 0; i < my_input.length(); i++)
    {        
        switch(int(arr[i])) {
          case -118:
            my_html_output.append("&egrave;");
            break;                
        case -126:
            my_html_output.append("&eacute");
            break;
          case -44:
            my_html_output.append("&Egrave;");
            break;                
        case -112:
            my_html_output.append("&Eacute;");
            break;   
          case -119:
            my_html_output.append("&euml;");
            break;                
        case -45:
            my_html_output.append("&Euml;");               
          case -123:
            my_html_output.append("&agrave;");
            break;
          case -73:
            my_html_output.append("&Agrave;");
            break;                              
          case 38:
            my_html_output.append("&amp;");
            break;   
          default:
            my_html_output.push_back(arr[i]);
        }        

                  
    }        

    return my_html_output;   
}


/********************************************************************
void BP_readFileCaption()
Returns caption.html as a string
Thanks to the guys on 
https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring/2602258
*********************************************************************/
string BP_readFileCaption()
{
    stringstream str;
    ifstream stream("caption.html");
    if(stream.is_open())
    {
        while(stream.peek() != EOF)
        {
            str << (char) stream.get();
        }
        stream.close();
        return str.str();
    }
}

    
    
    