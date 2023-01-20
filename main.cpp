#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main()
{

    try//Begin process
    {

        //declare stream/file variables
        string inFileLine, inFileName, outFileName;
        ifstream  inputStream;
        ofstream  outputStream;


        //https://stackoverflow.com/questions/11794144/regular-expression-for-valid-inFileName
        regex mask("^[a-zA-Z0-9](?:[a-zA-Z0-9 ._-]*[a-zA-Z0-9])?\\.[a-zA-Z0-9_-]+$");
        bool validName = false;

        //get the file name from the user and validate it against the regex mask
        while (!validName)
        {
            cout << "Please enter the name of the file you wish to convert e.g \"test.cpp\" : " << endl;
            cin >> inFileName;

            cout << "Please provide a name for your generated .html file e.g \"myOutputFile\" : " << endl;
            cin >> outFileName;
            outFileName +=  ".html";

            //if no input prompt the user
            if (!cin)
                cout << "Please enter a file name. e.g \"myInputFile.cpp\" : " << endl;

            //check file name against regex mask, if it matches it means the file name is valid.
            if (regex_match(inFileName,mask))
            {
                cout << "Valid file name. Attempting to open file" << inFileName << "...." << endl;
                validName = true;
            }
            else
            {
                cout << "File name is invalid. Make sure you are including the file extension." << endl;
            }
        }

        //attempt to open the files
        inputStream.open(inFileName);
        outputStream.open(outFileName, ios::app);

        if (inputStream.is_open() && !outputStream.fail())
        {
            //if both files open, begin the conversion process
            cout << "Successfully opened "<< inFileName << ". beginning conversion" << endl;

            string lessReplacement = "&lt;", greaterReplacement = "&gt;";

            //adding pre tag to the top of the file
            outputStream << "<PRE>" << endl;

            //Steps of conversion process
            //1.Takes a line from the input file.
            //2.Goes through each character of the current line and searches for > or < symbols.
            //3.If a symbol is found replaces the symbol with "&gt" or "&lt".
            //  If no match is found, continue scanning the line.
            //4.Writes the converted line to the output file.
            //  This process continues until there are no more lines of text to scan.

            while (!inputStream.eof())
            {
                getline(inputStream,inFileLine);
                for (int letter = 0; letter < inFileLine.size(); letter++) {

                    if (inFileLine[letter] == '<'){
                        inFileLine.replace(letter,1,lessReplacement);
                    }
                    if (inFileLine[letter] == '>'){
                        inFileLine.replace(letter,greaterReplacement.size(),greaterReplacement);
                    }
                }
                cout << inFileLine << endl;
                outputStream << inFileLine << endl;
            }

        }
        else //if either file failed to open, prompt the user
        {
            if (inputStream.fail()){
                cout <<"Failed to open " << inFileName << " ." << endl;
            }

            if (outputStream.fail()){
                cout <<"Failed to open " << inFileName << "." << endl;
            }

            return 0;
        }

        //add closing pre tag to bottom of the file
        outputStream << "</PRE>" << endl;


        //close the files
        outputStream.close();
        inputStream.close();


    }catch (const std::ios_base::failure& fail)
    {
        cout << "Error occurred during fileIO process." << endl;
    }//https://en.cppreference.com/w/cpp/io/basic_ios/exceptions

    return 0;
}
