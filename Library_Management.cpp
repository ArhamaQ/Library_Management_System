
/* This is a mini project devloped to manage the Library operations including the book records as well as student records*/

#include<iostream>
#include<cstdlib>
#include<fstream>
#include<iomanip>
#include<utility>
#include<stdexcept>
#include<ctime>
#include<regex>
#include<cstdio>

using namespace std;

//Forward Class declaration 

class Library;
class LibraryRecords;
class Student;

//Date class for Handeling the dates in standard format

class Date{
    private:
        int dd,mm,yr;

    public:
        Date(){
            this->dd = 0;
            this->mm = 0;
            this->yr = 0;
        }    

        Date(int dd,int mm ,int yr){
            this->dd=dd;
            this->mm=mm;
            this->yr=yr;
        }

    bool operator==(Date&);
    int operator-(Date&);

    friend istream& operator >>(istream &,Date &);
    friend ostream& operator <<(ostream &,const Date &);
    friend class LibraryRecords;
    friend class Student;
};

//overloaded == operator to compare Dates

bool Date ::operator==(Date& dt){
    if(dd==dt.dd && mm==dt.mm && yr==dt.yr){
        return true;
    }
    return false;
}

//overloaded - operator to calculate the difference between two dates

int Date :: operator-(Date& dt){
    
    int tempdd, tempmm, tempyy;
    tempyy=yr-dt.yr;
    tempmm=mm-dt.mm;
    tempmm<0?tempyy-=1:tempyy;
    tempdd=dd-dt.dd;
    return abs((tempyy*365)-((tempmm<0)?(tempmm+11)*30:(tempmm*30))-((tempdd<0)?(tempdd+30):tempdd));
}

//overloaded << operator to work with variables of Date class

ostream& operator <<(ostream &out ,const Date &dt){
    out<<dt.dd<<" "<<dt.mm<<" "<<dt.yr;
    return out;
}

//overloaded >> operator to work with variables of Date class

istream& operator >>(istream &in, Date &dt){
    in>>dt.dd>>dt.mm>>dt.yr;
    return in;
}

//abstract class containing genral attributes for different type of books

class Book{
    protected:
        int bookCode;
        string bookName;
        string bookAuthor;
        string bookPublication;
        string bookUniqueCode;

    public:
        Book(){
            this->bookCode        = 0;
            this->bookName        = " ";
            this->bookAuthor      = " ";
            this->bookPublication = " ";
        }

        Book(string bookName,int bookCode,string bookAuthor, string bookPublication){
            this->bookCode        = bookCode;
            this->bookName        = bookName;
            this->bookAuthor      = bookAuthor;
            this->bookPublication = bookPublication;
        }


        //pure virtual function

        virtual void showBookDetails(int)=0;
               
};


//EngineeringBooks class derived from Book class to deal with books for engineering course only

class EngineeringBooks : public Book{
    public:
        EngineeringBooks(){}

        EngineeringBooks(string bookName,int bookCode,string bookAuthor, string bookPublication) : Book(bookName,bookCode,bookAuthor,bookPublication){

            this->bookUniqueCode = "ENGG"+ to_string(bookCode);         
        }

        void showBookDetails(int);
        void storeBookRecords(int);
        int retrieveBookRecords(EngineeringBooks&);
        void retrieveAllBooks();
        void updateBookRecords(EngineeringBooks&,int);
        
        bool searchBook(EngineeringBooks&);
        bool operator ==(EngineeringBooks&);

    friend Library;    
    friend LibraryRecords;

    friend ostream& operator<<(ostream& ,  const EngineeringBooks& );
    friend istream& operator>>(istream& , EngineeringBooks& );

};


//overloading == operator to compare books of same type

bool EngineeringBooks::operator==(EngineeringBooks& bk){
    if(bookCode==bk.bookCode && bookName==bk.bookName && bookAuthor==bk.bookAuthor && bookPublication==bk.bookPublication && bookUniqueCode==bk.bookUniqueCode){
        return true;
    }
    return false;
}

//overloading << operator to work with variables of EngineeringBooks

ostream& operator<<(ostream& os,  const EngineeringBooks& bk){
    os<<bk.bookCode<<" "<<bk.bookName<<" "<<bk.bookAuthor<<" "<<bk.bookPublication<<" "<<bk.bookUniqueCode<<endl;
    return os;
}

//overloading >> operator to work with variables of EngineeringBooks

istream& operator>>(istream& in, EngineeringBooks& bk){
    in>>bk.bookCode>>bk.bookName>>bk.bookAuthor>>bk.bookPublication>>bk.bookUniqueCode;
    return in;
}

//Function to store the details of engineering book in file

void EngineeringBooks :: storeBookRecords(int bookCount){
    ofstream fout("EngineeringBookRecords.txt",ios::app);
    fout<<bookCode<<" "<<bookName<<" "<<bookAuthor<<" "<<bookPublication<<" "<<bookUniqueCode<<" "<<bookCount<<endl;
    fout.close();
   
}


//Function to fetch the details of engineering book from the file

int EngineeringBooks :: retrieveBookRecords(EngineeringBooks& book){
    int bookCount=0;

    ifstream fin("EngineeringBookRecords.txt",ios::in);

    if(fin){
        fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        while(!fin.eof()){
            if(*this==book){
                cout<<left<<setw(35)<<" Book Available "<<endl;
                return bookCount;
            }
            fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        }
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }
    return bookCount;
}

//Function to update the records in file whenever a book is issued or returned by a student

void EngineeringBooks :: updateBookRecords(EngineeringBooks& book,int updatedBookCount){
    int bookCount=0;
    ofstream fout("EngineeringBookRecords1.txt",ios :: out);
    ifstream fin("EngineeringBookRecords.txt",ios :: in);

    if(fin){
        fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        while(!fin.eof()){
            if(book==*this){
                fout<<bookCode<<" "<<bookName<<" "<<bookAuthor<<" "<<bookPublication<<" "<<bookUniqueCode<<" "<<updatedBookCount<<endl;
            }
            else{
                fout<<bookCode<<" "<<bookName<<" "<<bookAuthor<<" "<<bookPublication<<" "<<bookUniqueCode<<" "<<bookCount<<endl;        
            }
            fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        }

        fout.close();
        fin.close();

        remove("EngineeringBookRecords.txt");
        rename("EngineeringBookRecords1.txt","EngineeringBookRecords.txt");
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }
}

//Function to check if a book entered by a student exists in our files or not

bool EngineeringBooks :: searchBook(EngineeringBooks& book){
    int bookCount=0;
    ifstream fin("EngineeringBookRecords.txt",ios::in);

    if(fin){
        fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        while(!fin.eof()){
            if(*this==book){
                return true;
            }
            fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        }
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    } 
    return false;

}

//Function to retrieve all book details from file

void EngineeringBooks :: retrieveAllBooks(){
    int bookCount;
    ifstream fin("EngineeringBookRecords.txt",ios::in);

    if(fin){
        fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        while(!fin.eof()){
            showBookDetails(bookCount);
            fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        }    
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }
    
}

//Function to show the books and its details

void EngineeringBooks::showBookDetails(int bookCount){
    cout<<left<<setw(5)<<bookCode<<left<<setw(15)<<bookName<<left<<setw(15)<<bookAuthor<<left<<setw(15)<<bookPublication<<left<<setw(10)<<bookUniqueCode<<left<<setw(10)<<bookCount<<endl;
}

//ManagementBooks class derived from Book class to deal with books for management course only

class ManagementBooks : public Book{
    private:
        string bookCategory;

    public:
        ManagementBooks(){}

        ManagementBooks(string bookName,int bookCode,string bookAuthor, string bookPublication):Book(bookName,bookCode,bookAuthor,bookPublication){
            this->bookUniqueCode = "MGMT" + to_string(bookCode);         
        }

        void showBookDetails(int);
        void storeBookRecords(int);
        int retrieveBookRecords(ManagementBooks&);
        void retrieveAllBooks();
        void updateBookRecords(ManagementBooks&,int);
        
        bool searchBook(ManagementBooks&);
        bool operator ==(ManagementBooks&);

        // bool operator ==(Book &book);

    friend Library;   
    friend LibraryRecords; 

    friend ostream& operator<<(ostream& ,  const ManagementBooks& );
    friend istream& operator>>(istream& , ManagementBooks& );
};

//overloading == operator to compare books of same type

bool ManagementBooks::operator==(ManagementBooks& bk){
    if(bookCode==bk.bookCode && bookName==bk.bookName && bookAuthor==bk.bookAuthor && bookPublication==bk.bookPublication && bookUniqueCode==bk.bookUniqueCode){
        return true;
    }
    return false;
}

//overloading << operator to work with variables of ManagementBooks

ostream& operator<<(ostream& os,  const ManagementBooks& bk){
    os<<bk.bookCode<<" "<<bk.bookName<<" "<<bk.bookAuthor<<" "<<bk.bookPublication<<" "<<bk.bookUniqueCode<<endl;
    return os;
}

//overloading >> operator to work with variables of ManagementBooks

istream& operator>>(istream& in, ManagementBooks& bk){
    in>>bk.bookCode>>bk.bookName>>bk.bookAuthor>>bk.bookPublication>>bk.bookUniqueCode;
    return in;
}

//Function to store the details of management book in file

void ManagementBooks :: storeBookRecords(int bookCount){
    ofstream fout("ManagementBookRecords.txt",ios::app);
    fout<<bookCode<<" "<<bookName<<" "<<bookAuthor<<" "<<bookPublication<<" "<<bookUniqueCode<<" "<<bookCount<<endl;
    fout.close();   
}

//Function to fetch the details of management book from the file

int ManagementBooks :: retrieveBookRecords(ManagementBooks& book){
    int bookCount=0;

    ifstream fin("ManagementBookRecords.txt",ios::in);

    if(fin){
        fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        while(!fin.eof()){
            if(*this==book){
                cout<<left<<setw(35)<<" Book Available "<<endl;
                return bookCount;
            }
            fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        }
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }

    return bookCount;
}

//Function to update the records in file whenever a book is issued or returned by a student

void ManagementBooks :: updateBookRecords(ManagementBooks& book,int updatedBookCount){
    int bookCount=0;
    ofstream fout("ManagementBookRecords1.txt",ios :: out);
    ifstream fin("ManagementBookRecords.txt",ios :: in);
    
    if(fin){

        fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        while(!fin.eof()){
            if(book==*this){
                fout<<bookCode<<" "<<bookName<<" "<<bookAuthor<<" "<<bookPublication<<" "<<bookUniqueCode<<" "<<updatedBookCount<<endl;
            }
            else{
                fout<<bookCode<<" "<<bookName<<" "<<bookAuthor<<" "<<bookPublication<<" "<<bookUniqueCode<<" "<<bookCount<<endl;        
            }
            fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        }
        fout.close();
        fin.close();

        remove("ManagementBookRecords.txt");
        rename("ManagementBookRecords1.txt","ManagementBookRecords.txt");
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }    

}

//Function to check if a book entered by a student exists in our files or not

bool ManagementBooks :: searchBook(ManagementBooks& book){
    int bookCount=0;
    ifstream fin("ManagementBookRecords.txt",ios::in);

    if(fin){
        fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        while(!fin.eof()){
            if(*this==book){
                return true;
            }
            fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        }
            
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }

    return false;

}

//Function to retrieve all book details from file

void ManagementBooks :: retrieveAllBooks(){
    int bookCount;
    ifstream fin("ManagementBookRecords.txt",ios::in);
    if(fin){
        fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        while(!fin.eof()){
            showBookDetails(bookCount);
            fin>>bookCode>>bookName>>bookAuthor>>bookPublication>>bookUniqueCode>>bookCount;
        }
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }   
}

//Function to show the books and its details

void ManagementBooks::showBookDetails(int bookCount){
    cout<<left<<setw(5)<<bookCode<<left<<setw(15)<<bookName<<left<<setw(15)<<bookAuthor<<left<<setw(15)<<bookPublication<<left<<setw(10)<<bookUniqueCode<<left<<setw(10)<<bookCount<<endl;
}

//Student class to handle the record of a student

class Student{
    private:
        string stuName;
        string stuCourse;
        int enrollNo;    
    public:
        Student(){}

        Student(string stuName, string stuCourse, int enrollNo){
            this->stuName   = stuName;
            this->stuCourse = stuCourse;
            this->enrollNo  = enrollNo;
        }  

        bool operator ==(Student&);

    friend ostream& operator<<(ostream& ,  const Student&);
    friend istream& operator>>(istream& , Student& );


    friend LibraryRecords;       
};

//overloading == operator to match the student type variable

bool Student :: operator ==(Student& st){
    if(stuName==st.stuName && stuCourse==st.stuCourse && enrollNo== st.enrollNo){
        return true;
    }

    return false;
}

//overloading << operator to work with variables of student class

ostream& operator<<(ostream& out,  const Student& st){
    out<<st.stuName<<" "<<st.stuCourse<<" "<<st.enrollNo<<endl;
    return out;
}

//overloading >> operator to work with variables of student class

istream& operator>>(istream& in, Student& st){
    in>>st.stuName>>st.stuCourse>>st.enrollNo;
    return in;
}

//LibraryRecords  class to manage the Library Records

class LibraryRecords{
    private:
        ManagementBooks  mBook;
        EngineeringBooks eBook;
        Student student;
        Date dt;
        float fine;


    public:
        LibraryRecords(){}

        LibraryRecords(EngineeringBooks eBook, ManagementBooks mBook, Student student, Date dt){
            this->eBook        = eBook;
            this->mBook        = mBook;
            this->student      = student;
            this->dt           = dt;
            this->fine         = 0;
        }

      

    void storeLibraryRecords(int );
    bool searchRecord(LibraryRecords&,int);
    bool operator==(LibraryRecords&);
    void showRecord();
    void updateLibraryRecord(LibraryRecords&,int);
    void retrieveAllLibraryRecords();
    int calculateFine(int, int);
    

    friend ostream& operator <<(ostream &, LibraryRecords&);
    friend istream& operator >>(istream &, LibraryRecords&);
    friend Library;

};

//overloading == operator  to match library records

bool LibraryRecords :: operator==(LibraryRecords& libRec){
    if(eBook==libRec.eBook && student==libRec.student && dt==libRec.dt){
        return true;
    }
    else if(mBook==libRec.mBook && student==libRec.student && dt==libRec.dt){
        return true;
    }
    return false;
}

//overloading << operator to work with variables of library records

ostream& operator <<(ostream &out, LibraryRecords &libRec){
    out<<libRec.eBook<<endl<<libRec.mBook<<endl<<libRec.student<<endl<<libRec.dt<<endl<<libRec.fine;
    return out;
}

//overloading >> operator to work with variables of library records

istream& operator >>(istream &in, LibraryRecords &libRec){
    in>>libRec.eBook>>libRec.mBook>>libRec.student>>libRec.dt>>libRec.fine;
    return in;
}

//Function to show the library records

void LibraryRecords::showRecord(){
    cout<<mBook<<"  "<<eBook<<"  "<<student<<"  "<<dt<<"  "<<fine<<endl;
}

//Function to store the library records in a file

void LibraryRecords::storeLibraryRecords(int bookChoice){
    ofstream fout("LibraryRecords.txt",ios::app);
    if(bookChoice==1){
        fout<<eBook<<student<<dt<<endl<<fine<<endl;
    }
    else{
        fout<<mBook<<student<<dt<<endl<<fine<<endl;
    }
    fout<<endl;
}

//Function to search if a record of a particular student and book exist in library records or not 

bool LibraryRecords :: searchRecord(LibraryRecords& libRec, int bookChoice){
   
    ifstream fin("LibraryRecords.txt",ios::in);
    
    if(fin){
        if(bookChoice==1){
            fin>>eBook>>student>>dt>>fine;
            while(!fin.eof()){
            if(libRec==*this){
                showRecord();
                return true;
            }
            fin>>eBook>>student>>dt>>fine;
            }   
        }
        else if(bookChoice==2){
            fin>>mBook>>student>>dt>>fine;
            while(!fin.eof()){
            if(libRec==*this){
                showRecord();
                return true;
            }
            fin>>mBook>>student>>dt>>fine;
            }   
        }
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }    

    return false;   

}

//Function to update library records whenever a student issues or returns a book

void LibraryRecords :: updateLibraryRecord(LibraryRecords& libRec, int bookChoice){
    ofstream fout("LibraryRecords1.txt",ios::out);
    ifstream fin("LibraryRecords.txt",ios::in);
    
    if(fin){ 
        if(bookChoice==1){
            fin>>eBook>>student>>dt>>fine;
            while(!fin.eof()){
                if(!(libRec==*this)){
                    fout<<eBook<<" "<<student<<" "<<dt<<" "<<fine<<endl;
                }
                fin>>eBook>>student>>dt>>fine;
            }
            
        }

        else if(bookChoice==2){
            fin>>mBook>>student>>dt>>fine;
            while(!fin.eof()){
                if(!(libRec==*this)){
                    fout<<mBook<<" "<<student<<" "<<dt<<" "<<fine<<endl;
                }
                fin>>mBook>>student>>dt>>fine;
            }
        }
        
        fout.close();
        fin.close();
        remove("LibraryRecords.txt");
        rename("LibraryRecords1.txt","LibraryRecords.txt");
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }    
}

//Function to retrieve all library records from a file

void LibraryRecords :: retrieveAllLibraryRecords(){
    ifstream fin("LibraryRecords.txt",ios::in);
    
    if(fin){
        fin>>eBook>>student>>dt>>fine;
        while(!fin.eof()){
            showRecord();
            fin>>eBook>>student>>dt>>fine;
        }

        fin>>mBook>>student>>dt>>fine;
        while(!fin.eof()){
            showRecord();
            fin>>mBook>>student>>dt>>fine;
        }
        fin.close();    
    }
    else{
        cout<<left<<setw(35)<<" No Record Found"<<endl;
    }

}

//Function to calculate the fine on a student when he returns book

int LibraryRecords :: calculateFine(int dayDiff, int bookChoice){
    if(dayDiff > 7){
        fine =(dayDiff-7)*10;
        cout<<fine<<endl;
        return fine;
    }
    else{
        fine=0; 
    }
    return fine;
}

//Library class to handle all the library operations

class Library{
    private:
       ManagementBooks mBook;
       EngineeringBooks eBook;
       Student student;
       LibraryRecords libRec;
       int maxBookCount;

    public:
        Library(){
            maxBookCount=10;
        }

        void libraryMenu();   
        void addBook();
        void issueBook();
        void returnBook();
        Date returnValidDate();
        void storeBookDetails();

    friend ostream& operator<<(ostream& os,  Library& lib);
    
};

//function to display all the options available to a user

void Library::libraryMenu(){
    string str;
    int choice;
    do{
        fflush(stdin);
        system("CLS");
        cout<<left<<setw(35)<<"1.  Add Book     "<<endl;
        cout<<left<<setw(35)<<"2.  Issue Book   "<<endl;
        cout<<left<<setw(35)<<"3.  Return Book  "<<endl;
        cout<<left<<setw(35)<<"4.  Available Books   "<<endl;
        cout<<left<<setw(35)<<"5.  Show Library Records   "<<endl;
        cout<<left<<setw(35)<<"6.  Exit         "<<endl;
        cout<<left<<setw(40)<<" Enter Your Choice    ";
        cin>>choice;

        switch(choice){
            case 1 : addBook();
                     cout<<left<<setw(35)<<" Press Enter"<<endl;
                     cin.get();
                     cin.get();           
            break;

            case 2 : issueBook();
                     cout<<left<<setw(35)<<" Press Enter"<<endl;
                     cin.get();
                     cin.get();
            break;
            
            case 3 : returnBook();
                     cout<<left<<setw(35)<<" Press Enter"<<endl;
                     cin.get();
                     cin.get();
            break;

            case 4 : cout<<left<<setw(5)<<"Code"<<left<<setw(15)<<"Book Name"<<left<<setw(15)<<"Author"<<left<<setw(15)<<"Publisher"<<left<<setw(10)<<"UCode"<<left<<setw(10)<<"Quantity"<<endl;
                     mBook.retrieveAllBooks();
                     eBook.retrieveAllBooks();
                     cout<<left<<setw(35)<<" Press Enter"<<endl;
                     cin.get();
                     cin.get();
            break;

            case 5 : libRec.retrieveAllLibraryRecords();
                     cout<<left<<setw(35)<<" Press Enter"<<endl;
                     cin.get();
                     cin.get();
            break;

            case 6 : exit(0);
            break;

            default: cout<<"Invalid Option!"<<endl;

        }
    }while(1);    


}

//function to check if a date entered by a user is valid or not

Date Library :: returnValidDate(){
    int dd,mm,yy;
    int currYear;


    time_t now = time(0);
    tm *localTime = localtime(&now);
    currYear = 1900 +(localTime->tm_year);


    try{
                
        cout<<left<<setw(35)<<" Enter Issue Date (dd/mm/yy) "<<left<<setw(4)<<" : ";
        cin>>dd>>mm>>yy;

        if((dd>29 || dd<=0) && (mm==2)){
            throw runtime_error("Invalid Date !");
        }

        else if(((dd>31 || dd<=0) && (mm>6 || mm%2==0)) || ((dd>31 || dd<=0) && (mm<6 || mm%2!=0))){
            throw runtime_error("Invalid Date !");
        }

        else if(((dd>30 || dd<=0) && (mm==4 || mm==6)) || ((dd>30 || dd<=0) && (mm==9 || mm==11))){
            throw runtime_error("Invalid Date !");
        }

        if((yy>currYear || yy<2000)){
            throw runtime_error("Invalid Date !");
        }
                
    }catch(runtime_error &error){
        cout<<left<<setw(35)<<" Exception Occured !"<<endl;
        libraryMenu();
    }

    Date tempDate(dd,mm,yy);

    return tempDate;
}


//function to add book in library

void Library :: addBook(){

    string bookName;
    int bookCode;
    string bookAuthor;
    string bookPublication;

    int bookChoice;
    string input;
    regex stringExpr("[A-Z_a-z]+");

    cout<<left<<setw(35)<<" 1 Engineering "<<endl;
    cout<<left<<setw(35)<<" 2 Management"<<endl;
    cout<<left<<setw(40)<<" Enter Book Category ";
    cin>>bookChoice;
    
    while(true){
        cout<<left<<setw(35)<<" Enter Book Name "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookName=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(35)<<" Enter Book Author "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookAuthor=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    

    while(true){
        cout<<left<<setw(35)<<" Enter Publisher of Book "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookPublication=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    
    cout<<left<<setw(35)<<" Enter Book Code "<<left<<setw(4)<<" : ";
    cin>>bookCode;


    if(bookChoice==1){
        EngineeringBooks tempBook(bookName,bookCode,bookAuthor,bookPublication);
        eBook = tempBook;

        eBook.storeBookRecords(maxBookCount);

        cout<<left<<setw(35)<<" Book Added Successfully "<<endl;

        cout<<left<<setw(5)<<"Code"<<left<<setw(15)<<"Book Name"<<left<<setw(15)<<"Author"<<left<<setw(15)<<"Publisher"<<left<<setw(10)<<"UCode"<<left<<setw(10)<<"Quantity"<<endl;

        eBook.retrieveAllBooks();
    }
    else{
        ManagementBooks tempBook(bookName,bookCode,bookAuthor,bookPublication);
        mBook = tempBook;

        mBook.storeBookRecords(maxBookCount);

        cout<<left<<setw(35)<<" Book Added Successfully "<<endl;

        cout<<left<<setw(5)<<"Code"<<left<<setw(15)<<"Book Name"<<left<<setw(15)<<"Author"<<left<<setw(15)<<"Publisher"<<left<<setw(10)<<"UCode"<<left<<setw(10)<<"Quantity"<<endl;

        mBook.retrieveAllBooks();
    }
    

}

//function to issue a book from the library

void Library :: issueBook(){

    string stuName;
    string stuCourse;
    int enrollNo;

    regex stringExpr("[A-Z_a-z]+");
    string input;

    cout<<left<<setw(35)<<" Enter Your Enrollment Number "<<left<<setw(4)<<" : ";
    cin>>enrollNo;

    while(true){
        cout<<left<<setw(35)<<" Enter Your Name "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            stuName=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    } 
    
    while(true){
        cout<<left<<setw(35)<<" Enter Your Course "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            stuCourse=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    
    string bookName;
    int bookCode;
    string bookAuthor;
    string bookPublication;

    int bookChoice;
    
    
    cout<<left<<setw(35)<<" 1 Engineering "<<endl;
    cout<<left<<setw(35)<<" 2 Management"<<endl;
    cout<<left<<setw(40)<<" Enter Book Category ";
    cin>>bookChoice;

    while(true){
        cout<<left<<setw(35)<<" Enter Book Name "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookName=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(35)<<" Enter Book Author "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookAuthor=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(35)<<" Enter Publisher of Book "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookPublication=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    
    cout<<left<<setw(35)<<" Enter Book Code "<<left<<setw(4)<<" : ";
    cin>>bookCode;

    if(bookChoice == 1){
        EngineeringBooks tempbook(bookName,bookCode,bookAuthor,bookPublication); 
        if(eBook.searchBook(tempbook)){
            int bookCount = eBook.retrieveBookRecords(tempbook);

            if(bookCount==0){
                cout<<left<<setw(35)<<" Cannot issued book at this moment "<<endl;
                eBook.retrieveAllBooks();
                libraryMenu();
            }


            Date tempDate=returnValidDate(); 

            bookCount--;
            eBook.updateBookRecords(tempbook,bookCount);

            Student tempstudent(stuName,stuCourse,enrollNo);

            LibraryRecords tempRecord(tempbook,mBook,tempstudent,tempDate);
            tempRecord.storeLibraryRecords(bookChoice);
        }
        else{
            cout<<left<<setw(35)<<" Sorry, Book does not exist !!"<<endl;
        }   
    }
    else if(bookChoice == 2){
        ManagementBooks tempbook(bookName,bookCode,bookAuthor,bookPublication);
        if(mBook.searchBook(tempbook)){
            int bookCount = mBook.retrieveBookRecords(tempbook);

            if(bookCount==0){
                cout<<left<<setw(35)<<" Cannot issued book at this moment "<<endl;
                mBook.retrieveAllBooks();
                libraryMenu();
            }
            
            Date tempDate=returnValidDate();            

            bookCount--;
            mBook.updateBookRecords(tempbook,bookCount);

            Student tempstudent(stuName,stuCourse,enrollNo);

            LibraryRecords tempRecord(eBook, tempbook, tempstudent, tempDate);
            tempRecord.storeLibraryRecords(bookChoice);
        }
        else{
            cout<<left<<setw(35)<<" Sorry, Book does not exist !!"<<endl;
        }
    }   
    
}

//function to return book to library

void Library :: returnBook(){
   
    string stuName;
    string stuCourse;
    int enrollNo;

    regex stringExpr("[A-Z_a-z]+");
    string input;

    cout<<left<<setw(35)<<" Enter Your Enrollment Number "<<left<<setw(4)<<" : ";
    cin>>enrollNo;
    
    while(true){
        cout<<left<<setw(35)<<" Enter Your Name "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            stuName=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    } 
    
    while(true){
        cout<<left<<setw(35)<<" Enter Your Course "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            stuCourse=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    
    string bookName;
    int bookCode;
    string bookAuthor;
    string bookPublication;

    int bookChoice;
    
    
    cout<<left<<setw(35)<<" 1 Engineering "<<endl;
    cout<<left<<setw(35)<<" 2 Management"<<endl;
    cout<<left<<setw(40)<<" Enter Book Category ";
    cin>>bookChoice;

    while(true){
        cout<<left<<setw(35)<<" Enter Book Name "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookName=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(35)<<" Enter Book Author "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookAuthor=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(35)<<" Enter Publisher of Book "<<left<<setw(4)<<" : ";
        cin>>input;
        if(regex_match(input,stringExpr)){
            bookPublication=input;
            break;
        }
        else{
            cout<<left<<setw(35)<<" Invalid Input"<<endl;
        }
    }
    
    cout<<left<<setw(35)<<" Enter Book Code "<<left<<setw(4)<<" : ";
    cin>>bookCode;

    time_t now = time(0);
    tm *localTime = localtime(&now); 

    int rdd,rmm,ryy;
    ryy = 1900 +(localTime->tm_year);
    rmm = 1+(localTime->tm_mon);
    rdd = (localTime->tm_mday);

    int tempFine,actualFine;

    if(bookChoice == 1){
        EngineeringBooks tempbook(bookName,bookCode,bookAuthor,bookPublication); 
        if(eBook.searchBook(tempbook)){
            Date tempDate = returnValidDate(); 
            Date returnDate(rdd,rmm,ryy);
            int dayDiff = returnDate - tempDate;

            int bookCount=eBook.retrieveBookRecords(tempbook);
            bookCount++;
            if(bookCount>10){
                cout<<left<<setw(35)<<" Invalid Operation ! "<<endl;
                libraryMenu();
            }

            Student tempstudent(stuName,stuCourse,enrollNo);
            LibraryRecords tempRecord(tempbook, mBook,tempstudent,tempDate);

            actualFine = tempRecord.calculateFine(dayDiff,bookChoice);

            if(libRec.searchRecord(tempRecord,bookChoice)){
                if(actualFine > 0){
                   cout<<left<<setw(35)<<" Cannot return book, fine exist !"<<endl;
                   cout<<left<<setw(35)<<" Pay Fine "<<left<<setw(4)<<" : ";
                   cin>>tempFine;
                   while(tempFine!=actualFine){
                        cout<<left<<setw(35)<<" Insufficient Amount"<<left<<setw(4)<<" : ";
                        cin>>tempFine;
                   }
                   cout<<left<<setw(35)<<" Fine Collected"<<endl;
                }
                libRec.updateLibraryRecord(tempRecord,bookChoice);
                eBook.updateBookRecords(tempbook,bookCount);
                cout<<left<<setw(35)<<" Book returned Successfully"<<endl;

            }
            else{
                cout<<left<<setw(35)<<" Record Not Found ! "<<endl;
            }
        }
        else{
            cout<<left<<setw(35)<<" Sorry, Book does not exist !!"<<endl;
        }   
    }
    else if(bookChoice == 2){
        ManagementBooks tempbook(bookName,bookCode,bookAuthor,bookPublication);
        if(mBook.searchBook(tempbook)){

            Date tempDate=returnValidDate(); 
            Date returnDate(rdd,rmm,ryy);
            int dayDiff = returnDate - tempDate;

            int bookCount=mBook.retrieveBookRecords(tempbook);
            bookCount++;
            if(bookCount>10){
                cout<<left<<setw(35)<<" Invalid Operation ! "<<endl;
                libraryMenu();
            }

            Student tempstudent(stuName,stuCourse,enrollNo);

            LibraryRecords tempRecord(eBook,tempbook,tempstudent,tempDate);
            
            actualFine=tempRecord.calculateFine(dayDiff,bookChoice);

            if(libRec.searchRecord(tempRecord,bookChoice)){
                if(actualFine > 0){
                   cout<<left<<setw(35)<<" Cannot return book, fine exist !"<<endl;
                   cout<<left<<setw(35)<<" Pay Fine "<<left<<setw(4)<<" : ";
                   cin>>tempFine;
                    while(tempFine!=actualFine){
                        cout<<left<<setw(35)<<" Insufficient Amount"<<left<<setw(4)<<" : ";
                        cin>>tempFine;
                    }
                }
                libRec.updateLibraryRecord(tempRecord,bookChoice);
                mBook.updateBookRecords(tempbook,bookCount);
                cout<<left<<setw(35)<<" Book returned Successfully"<<endl;
            }
            else{
                cout<<left<<setw(35)<<" Record Not Found ! "<<endl;
            }
        }
        else{
            cout<<left<<setw(35)<<" Sorry, Book does not exist !!"<<endl;
        }
    }
    
}

//Driver Code

int main(){

    system("CLS");
    Library libraryOperation;
    libraryOperation.libraryMenu();

    return 0;
}
