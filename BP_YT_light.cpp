#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

#include <sys/types.h>
#include <dirent.h>

#include "color.hpp"

using namespace std;


// Misc functions, code stolen on the web.
// Please write me : bludit@jnss.eu if you're the author or in case of problem.
#include "misc.hpp"

// Bludit Post main functions.
#include "bp.hpp"


int main(int argc, char *argv[]){
    

	/* Choose config file number
    ****************************/        
    string cfg_num = BP_ChooseConfigFile();

    
	/* Read config file
	**************************/
    string strarray[8];
    
    BP_ReadConfigFile(cfg_num,strarray); 

    string usr_videoDivClass = strarray[0];
    string usr_Caption = strarray[1];
    string usr_Token = strarray[2];
	string usr_Auth = strarray[3];
	string usr_PageAPI = strarray[4];
	string usr_PageCats = strarray[5];

    string ht_UsrName = strarray[6]; 
    string ht_Pwd = strarray[7]; 

    
	/* Title prompt
	/**********************************/
    cout << "Page title?" << endl;
	string PageTitle;
	getline(cin, PageTitle);
	getline(cin, PageTitle);
    cout << endl;
    
    PageTitle = BP_HTMLformat(PageTitle);
    
	/* Category choice
	/*************************************************/
	string ArticleCat = BP_ChooseCategory(usr_PageCats);
	

    /* Prompts for video caption
    *******************************************/
    string post_caption;
    string usr_choice_caption;

    cout << "Enter 'y' if you want to type a caption, or 'n' to use caption.html contents:" << endl;
    cin >> usr_choice_caption;

    if(usr_choice_caption == "y")
    {                
        cout << "Post caption : " <<endl;
        getline(cin, post_caption);
        getline(cin, post_caption);        
    }
    else
    {    
        post_caption = BP_readFileCaption();
    
    }

    post_caption = BP_HTMLformat(post_caption);    
    

	/* Write HTML file, which will be put on the server,
    * replacing Bludit void content
	**************************************************/
	string const HTMLFileName("./index.txt");
	ofstream HtmlFileOutStream (HTMLFileName.c_str()); // ajouter  ,ios::app); pour l'ajout de texte dans l'existant (append)
	if(!HtmlFileOutStream)
	{
		cout << "ERROR: can't create ./index.txt." << endl;
		return 1;
	}
    
    //Prompts for YouTube URL
    string post_YT_URL;
    cout << "Youtube URL (eg. \"https://www.youtube.com/watch?v=cbP2N1BQdYc\"): " << endl;
    cin >> post_YT_URL;

    //Embeds it on our page
    size_t pos3 = post_YT_URL.find("=");
    post_YT_URL = post_YT_URL.substr (pos3+1);

    HtmlFileOutStream << "<div class=\"" << usr_videoDivClass <<"\"><iframe width=\"800\" height=\"447\" src=\"https://www.youtube.com/embed/";
    HtmlFileOutStream << post_YT_URL; 
    HtmlFileOutStream << "\" frameborder=\"0\" allow=\"accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe></p><br><br></div>";            


    //Caption
    HtmlFileOutStream << "<p class=\"" << usr_Caption << "\">";
    HtmlFileOutStream << post_caption << endl;             
    HtmlFileOutStream << "</p>";   
    
    //Close
	HtmlFileOutStream.close();
    
 
	/* Creates data.json file for CURL command
	**************************************************/
	string const JSONFileName("./data.json");
	ofstream JSONFileOutStream (JSONFileName.c_str()); // ajouter  ,ios::app); pour l'ajout de texte dans l'existant (append)
	if(!JSONFileOutStream)
	{
		cout << "ERROR: can't create ./data.json." << endl;
		return 1;
	}
	
    //Inputs data...
	JSONFileOutStream << "{" << endl;
	JSONFileOutStream << "\"token\": \"" << usr_Token << "\"," << endl;
	JSONFileOutStream << "\"authentication\": \"" << usr_Auth << "\"," << endl;
	JSONFileOutStream << "\"title\": \"" << PageTitle << "\"," << endl;

	//Asks user for no-index / no-follow
	string noindex;
	string nofollow;
	
	cout <<  "Enter 'y' for noindex or 'n' otherwise: " << endl;
	cin >> noindex;
	if(noindex == "y")
	{
		JSONFileOutStream << "\"noindex\":\"false\"," << endl;
	}
	
	cout <<  "Enter 'y' for nofollow or 'n' otherwise: " << endl;
	cin >> nofollow;	
	if(nofollow == "y")
	{
		JSONFileOutStream << "\"nofollow\":\"false\"," << endl;
	}
	
	
    //If a category has been chosen, adds it
	if(ArticleCat != "") 
		JSONFileOutStream << "\"category\": \"" << ArticleCat << "\"," << endl;


    /*********************************************************************
    * If we want YT thumbnail, we generate an URL
    **********************************************************************/   
    JSONFileOutStream << "\"coverImage\": \"";
    JSONFileOutStream << "https://img.youtube.com/vi/";
    JSONFileOutStream << post_YT_URL;
    JSONFileOutStream << "/0.jpg\","  << endl;

    
    //No content, it'll be uploaded as 'index.txt'				
	JSONFileOutStream << "\"content\":\"\"" << endl;
	

    //Prompts user for article tags, add them if any.
    string my_tags = BP_ArticleTags();
    if(my_tags != "none")
	   JSONFileOutStream << ",\"tags\":\"" << my_tags << "\"" << endl;


    //End
    JSONFileOutStream << "}" << endl;
	JSONFileOutStream.close();
		
			
	/* Runs Curl command
	********************/
    string temp_key = BP_RunCurlJSON(usr_PageAPI,ht_UsrName,ht_Pwd);
    
	/* Replace index.txt of our new page
	************************************/
    BP_UploadIndexTXT(cfg_num,temp_key);

    
	return 0;
}