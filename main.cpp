#include "exception.h"

int main()
{
    //Begin process

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
        cout << "Please enter the full path and file name WITH extension to the file you wish to convert. e.g C:\\home\\testFile.cpp : " << endl;
        cin >> inFileName;

        cout << "Please provide the full path and file name WITHOUT extension for your generated .html file e.g C:\\home\\testFile : " << endl;
        cin >> outFileName;
        outFileName +=  ".html";

        //if no input prompt the user
        if (!cin)
            cout << "Please enter a file name. e.g \"myInputFile.cpp\" : " << endl;

        try
        {
            //check file name against regex mask, if it matches it means the file name is valid.
            if (regex_match(inFileName,mask))
            {
                cout << "Valid file name. Attempting to open file" << inFileName << "...." << endl;
                validName = true;
            }
            else
            {
                throw MyCustomException();
            }
        }
        catch(MyCustomException& e)//Programmer exception.
        {
            cout << e.what() << endl;
        }
    }//End while

    //attempt to open the files
    try
    {
        inputStream.open(inFileName);
        outputStream.open(outFileName, ios::app);
    }
    catch (exception& e)//Standard exception
    {
        e.what();
    }

    if (!inputStream.fail() && !outputStream.fail())
    {
        //if both files open without error, begin the conversion process
        cout << "Successfully opened "<< inFileName << ". beginning conversion" << endl;

        string lessReplacement = "&lt;", greaterReplacement = "&gt;";

        //adding pre tag to the top of the file
        outputStream << "<PRE>" << endl;

        //Steps of conversion process
        //1.Take a line from the input file.
        //2.Go through each character of the current line and search for > or < symbols.
        //3.If a symbol is found write "&gt" or "&lt" to the output file and move on to the next letter.
        //  If no match is found, continue scanning the line.
        //4.Write the converted line to the output file.
        //  This process continues until there are no more lines of text to scan.

        while (!inputStream.eof())
        {
            getline(inputStream,inFileLine);

            for (int letter = 0; letter < inFileLine.length(); letter++)
            {
                if (inFileLine[letter] == '<')
                {
                    outputStream << lessReplacement;
                    continue;
                }
                else if (inFileLine[letter] == '>')
                {
                    outputStream << greaterReplacement;
                    continue;
                }
                outputStream << inFileLine[letter];
            }
            outputStream << endl;
        }
    }
    else //if either file fails to open, tell the user which file failed.
    {
        if (inputStream.fail()){
            cout <<"Failed to open " << inFileName << "." << endl;
        }

        if (outputStream.fail()){
            cout <<"Failed to open " << inFileName << "." << endl;
        }

        return 0;
    }

    //add closing pre tag to bottom of the file
    outputStream << "</PRE>" << endl;

    //close the files
    try
    {
        outputStream.close();
        inputStream.close();
    }catch (const ios_base::failure& fail)//file IO exception(library exception)
    {
        cout << "Error closing files.";
        return 1;
    }

    return 0;
}//end main


