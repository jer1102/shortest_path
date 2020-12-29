
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;
int not_visited = 999999999;
struct connection
{
 string name;
 string type;
 int a, b;
 double length;
 connection(string n, string t, int x, int y, double l)
 {
  name = n;
  type = t;
  a=x;
  b=y;
  length = l;
 }
 void print()
 { cout<<name<<" "<<type<<" "<<a<<" "<<b<<" "<<length<<"\n";}
};

struct intersection
{
 double longitude;
 double latitude;
 double distance;
 string state, place;
 vector <connection *> roads;
 double total_distance;
 bool visited;
 int connect;
 int num;
 string direction;
 intersection(double lo,double la, double d, string s, string p, int n)
 {
  longitude = lo;
  latitude = la;
  distance = d;
  state = s;
  place = p;
  total_distance = not_visited;
  visited = false;
  num = n;
  connect = 0;
  direction = "";
 }
 void print()
 {
  cout<<longitude<<" "<<latitude<<" "<<distance<<" "<<state<<" "<<place<<"  total distance is: "<<total_distance<<"\n";
 }
 void print_roads()
 {
  int x=roads.size();
  for(int i=0;i<x;i++)
   roads[i]->print();
 }
};
//	isempty: done
//	add: done
//	change priority: done
//	delete: done

vector <intersection *> map;
struct minheap{
 vector <int> data;
 int size;
 minheap(int i)
 {
  size = i;
  data.push_back(0);
 }
 bool isempty()
 {
  if(data.size()==1)
   return true;
  return false;
 } 
 void add(int i, double d)
 {
  size += 1;
  data.push_back(i);
  map[i]->total_distance = d;
  work_up(size);
 }
 void work_up(int pos)
 {
  if(pos==1)
   {}
  else if(map[data[pos]]->total_distance<map[data[pos/2]]->total_distance)
   {swap(pos,pos/2);
    work_up(pos/2);}
 }
 void swap(int x, int y)
 {
  int temp = data[x];
  data[x]=data[y];
  data[y]=temp;
 }
 intersection *  get(int x)
 {
  return map[x];
 }
 void change_priority(intersection * d, double x)
 {
  int i=0;
  for(i; i<size; i++)
   if(map[data[i]]==d)
    break;
  map[data[i]]->total_distance = x;
  work_up(i);
 }
  
 void pop_front()
 {
  data[1]=data[size];
  data.pop_back();
  size -=1;
  work_down(1);
 }
 void work_down(int x)
 {
  if(x*2 >= size)
  {}
  else if(map[data[x]]->total_distance > map[data[x*2]]->total_distance)
   if(map[data[x]]->total_distance > map[data[x*2+1]]->total_distance)
    {
     if (map[data[x*2]]->total_distance > map[data[x*2+1]]->total_distance)
      {
      swap(x,x*2+1);
      work_down(x*2+1);
      }
    else
     {
      swap(x,x*2);
      work_down(x*2);
     }
    }
   else
   {
    swap(x,x*2);
    work_down(x*2); 
   }
 else if(map[data[x]]->total_distance > map[data[x*2+1]]->total_distance)
  {
   swap(x,x*2+1);
   work_down(x*2+1);
  }
 }
 void show_size()
  {
   cout<<"the size is: "<<size<<"\n";
  }
 void print_all()
 {
  for(int i = 0; i<size; i++)
   map[data[i+1]]->print();
 }
 
};

bool isNumeric(string s)
{
 for(int i=0; i<s.length();i++)
 {
  if(isdigit(s[i]) == false)
   return false;
 }
 return true;
}

bool isequal(double x, double y)
 
{
 if(x-y<.0001&&x-y>-.0001)
  return true;
 return false;
}

string get_direction(intersection * a, intersection * b)
{
 if(a->latitude - b->latitude < -.01)//a is more west
  if(a->longitude - b->longitude <-.01) //a is more south
   return "sw";
  else if(a->longitude - b->longitude >.01)//more north
   return "nw";
  else
   return "w";
 else if(a->latitude - b->latitude >.01)//more east
  if(a->longitude - b->longitude <-.01) //a is more south
   return "se";
  else if(a->longitude - b->longitude >.01)//more north
   return "ne";
  else
   return "e";
 else
  if(a->longitude - b->longitude < 0) //a is more south
   return "s";
  else 
   return "n";
  
}

int main()
{
 intersection * bingus = new intersection(0,0,0, " ", " ",0);
 connection * bongus = new connection(" "," ",0,0,0);
 vector<connection *> routes; 
 ifstream fin;
 fin.open("./data/intersections.txt");
 if (fin.fail())
 {
  cout<<"fail\n";
  exit(1);
 }
 double lo, lat, d;
 string s, p, temp, phldr;
 int counter = 0;
 while(true)
{
  getline(fin,temp);
  if(fin.fail())
   break;
  phldr.assign(temp,0,9);
  lo=stod(phldr);
  phldr.assign(temp,9,10);
  lat=stod(phldr);
  phldr.assign(temp,19,8);
  d=stod(phldr);
  s.assign(temp,28,2);
  p.assign(temp,31);
  bingus = new intersection(lo,lat,d,s,p,counter);
  map.push_back(bingus);
  counter ++;
}
 fin.close();
 fin.open("./data/connections.txt");
 if(fin.fail())
  {cout<<"error\n";
   exit(1);}
 string n,t;
 int a,b;
 double len;
 while(true)
 {
  fin>>n>>t>>a>>b>>len;
  if(fin.fail())
   break;
  bongus = new connection(n,t,a,b,len);
  map[a]->roads.push_back(bongus);
  map[b]->roads.push_back(bongus);  
 }
 delete bingus;
 delete bongus;
 minheap m(0);
 cout<<"minheap\n";
 int start, dest, here;
 cout<<"Where do you want to start? ";
 cin>> start;
 cout<<"where should it end? ";
 cin>> dest;
 m.add(start,0.0);//this is starting point
 while(!m.isempty())
  {
   here = m.data[1];
   if(m.data[1]==dest)
    {break;
    }
   for(int i =0; i<map[here]->roads.size();i++)
   {
    a=map[here]->roads[i]->a;
    if(map[a]->total_distance==not_visited)
     m.add(a,map[here]->roads[i]->length+map[here]->total_distance);
    else
     {
     //cout<<"1\n";
     if(map[here]->total_distance+map[here]->roads[i]->length < map[a]->total_distance)
     { m.change_priority(map[a],map[here]->total_distance+map[here]->roads[i]->length);
      //cout<<"change \n";
     }
    b=map[here]->roads[i]->b;
    if(map[b]->total_distance==not_visited)
     m.add(b,map[here]->roads[i]->length+map[here]->total_distance);
    else
     {
     //cout<<"2\n";
     if(map[here]->total_distance+map[here]->roads[i]->length < map[b]->total_distance)
      m.change_priority(map[b],map[here]->total_distance+map[here]->roads[i]->length);
      }
     }
    //m.show_size();
   }
    m.pop_front();
// m.print_all();
 }
 if(here!=dest)
  {cout<<"error place couldnt be reached\n";
   exit(1);}
 vector <intersection *> answer;
 answer.push_back(map[here]);
 cout<<"end of alg"<<"\n";
 while(true)
 {
  int h = here;
  if(here==start)
   break;
  if(map[here]->total_distance == not_visited)
   {cout<<"error\n";
    break;}
  for(int i = 0; i < map[here]->roads.size();i++)
  {
   
   double total =map[here]->total_distance - map[here]->roads[i]->length;
   
   //cout<<map[here]->roads[i]->length<<"\n";
   if(here == map[here]->roads[i]->a)
    {
      b=map[here]->roads[i]->b;
    if(isequal(total, map[b]->total_distance))
     {
       answer.push_back(map[here]);
     map[here]->connect = i;
     map[here]->direction = get_direction(map[here],map[b]);
     here = b;
     break;
     }
    }
   else
    {
    a=map[here]->roads[i]->a;
    if(isequal(total, map[a]->total_distance))
     {
       answer.push_back(map[here]);
     map[here]->connect = i;
     map[here]->direction = get_direction(map[here],map[b]);
     here = a;
     break;
     }
    }
   if(here!=h)
    break;
   }
  if(h==here)
   {
   cout<<"error no path found\n";
   exit(1);
  }
 }
  
 for(int i = answer.size()-1;i>0;i--){
  int x=answer[i]->connect;
  cout<<"From intersection ";
  if(answer[i]->roads[x]->a==answer[i]->num)
   cout<<answer[i]->roads[x]->b;
  else
   cout<<answer[i]->roads[x]->a;
  cout<<", take "<<answer[i]->roads[x]->name<<" "<<answer[i]->roads[x]->length<<" miles "<<answer[i]->direction;
  cout<<" to intersection "<<answer[i]->num<<"\n";;
    
}
}
