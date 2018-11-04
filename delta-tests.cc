//-------------------------------------------------------------------
//
// File:      delta-tests.cc
//
// @author    Carlos Baquero <cbm@di.uminho.pt>
//
// @copyright 2014 Carlos Baquero
//
// This file is provided to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file
// except in compliance with the License.  You may obtain
// a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
// @doc
//   Simple tests for the datatypes in delta-crdts.cc
// @end
//
//
//-------------------------------------------------------------------

#include <ctime>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
//#define NDEBUG  // Uncoment do stop testing asserts
#include <assert.h>
#include "delta-crdts.cc"

void test_gset()
{
  std::cout << "--- Testing: gset --\n";
  dtcrdt::gset<int> o1, o2, do1, do2;

  do1.join(o1.add(1));
  do1.join(o1.add(2));

  do2.join(o2.add(2));
  do2.join(o2.add(3));

  dtcrdt::gset<int> o3 = dtcrdt::join(o1, o2);
  dtcrdt::gset<int> o4 = dtcrdt::join(join(o1, do2), dtcrdt::join(o2, do1));
  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;
  std::cout << o3.in(1) << o3.in(0) << std::endl;

  dtcrdt::gset<std::string> o5;
  o5.add("hello");
  o5.add("world");
  o5.add("my");
  std::cout << o5 << std::endl;
}

void test_twopset()
{
  std::cout << "--- Testing: twopset --\n";
  dtcrdt::twopset<int> o1, o2, do1, do2;

  do1.join(o1.add(1));
  do1.join(o1.add(2));

  do2.join(o2.add(2));
  do2.join(o2.rmv(2));

  dtcrdt::twopset<int> o3 = dtcrdt::join(o1, o2);
  dtcrdt::twopset<int> o4 = dtcrdt::join(join(o1, do1), dtcrdt::join(o2, do1));
  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;
  std::cout << o3.in(1) << o3.in(2) << std::endl;

  dtcrdt::twopset<std::string> o5;
  o5.add("hello");
  o5.add("world");
  o5.add("my");
  o5.rmv("my");
  o5.rmv("my");
  std::cout << o5 << std::endl;
}

void test_gcounter()
{
  std::cout << "--- Testing: gcounter --\n";
  // default template type is std::string key and int value
  dtcrdt::gcounter<> o1("idx");
  dtcrdt::gcounter<> o2("idy");
  dtcrdt::gcounter<> do1, do2;

  do1.join(o1.inc());
  do1.join(o1.inc(4));

  do2.join(o2.inc());
  do2.join(o2.inc());

  dtcrdt::gcounter<> o3 = dtcrdt::join(o1, o2);
  dtcrdt::gcounter<> o4 = dtcrdt::join(join(o1, do1), dtcrdt::join(o2, do1));

  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;
  std::cout << o3.read() << std::endl;
}

void test_pncounter()
{
  std::cout << "--- Testing: pncounter --\n";
  // counter with ints in keys and floats in values
  dtcrdt::pncounter<float, int> o1(2);
  dtcrdt::pncounter<float, int> o2(5);
  dtcrdt::pncounter<float, int> do1, do2;

  do1.join(o1.inc(3.5));
  do1.join(o1.dec(2));

  do2.join(o2.inc());
  do2.join(o2.inc(5));

  dtcrdt::pncounter<float, int> o3 = dtcrdt::join(o1, o2);
  dtcrdt::pncounter<float, int> o4 = dtcrdt::join(join(o1, do2), dtcrdt::join(o2, do1));

  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;
  std::cout << o3.read() << std::endl;
}

void test_lexcounter()
{
  std::cout << "--- Testing: lexcounter --\n";
  dtcrdt::lexcounter<int, char> o1('a');
  dtcrdt::lexcounter<int, char> o2('b');
  dtcrdt::lexcounter<int, char> do1, do2;

  o1.inc(3);
  o1.inc(2);
  o1.dec(1);
  o2.inc(1);

  std::cout << o1 << std::endl;
  std::cout << o2 << std::endl;

  o2.join(o1);
  std::cout << o2.read() << std::endl;
}

void test_aworset()
{
  std::cout << "--- Testing: aworset --\n";
  dtcrdt::aworset<char> o1("idx"), o2("idy"), do1, do2;

  do1.join(o1.add('a'));
  do1.join(o1.add('b'));

  do2.join(o2.add('b'));
  do2.join(o2.add('c'));
  do2.join(o2.rmv('b'));

  dtcrdt::aworset<char> o3 = dtcrdt::join(o1, o2);
  dtcrdt::aworset<char> o4 = dtcrdt::join(join(o1, do1), dtcrdt::join(o2, do1));
  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;
  std::cout << o3.in('c') << o3.in('b') << std::endl;

  assert(o3.in('c') == true && o3.in('b') == true);

  dtcrdt::aworset<std::string> o5("idz");
  o5.add("hello");
  o5.add("world");
  o5.add("my");
  std::cout << o5 << std::endl;
}

void test_rworset()
{
  std::cout << "--- Testing: rworset --\n";
  dtcrdt::rworset<char> o1("id x"), o2("id y"), do1, do2;

  do1.join(o1.add('a'));
  do1.join(o1.add('b'));

  do2.join(o2.add('b'));
  do2.join(o2.add('c'));
  do2.join(o2.rmv('b'));

  dtcrdt::rworset<char> o3 = dtcrdt::join(o1, o2);
  dtcrdt::rworset<char> o4 = dtcrdt::join(join(o1, do1), dtcrdt::join(o2, do1));
  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;

  std::cout << o4.read() << std::endl;
  std::cout << o3.in('a') << o3.in('b') << std::endl;
}

void test_mvreg()
{
  std::cout << "--- Testing: mvreg --\n";
  dtcrdt::mvreg<std::string> o1("id x"), o2("id y"), do1, do2;

  do1.join(o1.write("hello"));
  do1.join(o1.write("world"));

  do2.join(o2.write("world"));
  do2.join(o2.write("hello"));

  dtcrdt::mvreg<std::string> o3 = dtcrdt::join(o1, o2);
  dtcrdt::mvreg<std::string> o4 = dtcrdt::join(join(o1, do1), dtcrdt::join(o2, do1));
  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;
  o3.write("hello world");
  o4.join(o3);
  std::cout << o4 << std::endl;

  std::cout << "--- Testing: mvreg with reduce --\n";

  dtcrdt::mvreg<int> o5("id x"), o6("id y"), o7("id z");

  o5.write(3);
  o6.write(5);
  o7.write(2);

  o5.join(o6);
  o5.join(o7);
  std::cout << o5.read() << std::endl;

  std::cout << o5.resolve() << std::endl;
  std::cout << o5.read() << std::endl;

  dtcrdt::mvreg<std::pair<int, int>> o8("id x"), o9("id y"), o10("id z");

  // notice that the default order for pairs is lexicographic in C++
  // std::cout << (std::pair<int,int>(0,1) < std::pair<int,int>(1,0)) << std::endl;

  o8.write(std::pair<int, int>(0, 0));
  o9.write(std::pair<int, int>(1, 0));
  o10.write(std::pair<int, int>(0, 1));

  o8.join(o9);
  o8.join(o10);
  std::cout << o8.read() << std::endl;

  std::cout << o8.resolve() << std::endl;
  std::cout << o8.read() << std::endl;
}

/*
void test_maxord()
{
  maxord<int> o1,o2,do1,do2;
  do1.join(o1.write(6)); 
  do1.join(o1.write(3)); 

  do2.join(o2=5); 
  do2.join(o2=10); 

  maxord<int> o3 = dtcrdt::join(o1,o2);
  maxord<int> o4 = dtcrdt::join(join(o1,do1),join(o2,do1));
  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;

  maxord<bool> o5;
  std::cout << o5 << std::endl;
  o5=true;
  o5=false; // Once it goes up, it cant go back
  std::cout << o5 << std::endl;

  
  maxord<double> a;
  maxord<double> b;
  std::cout << a << std::endl;
  a=1.1;
  std::cout << a << std::endl;
  b=(--a);
  std::cout << b << std::endl;
 

}
*/

void example1()
{
  dtcrdt::aworset<std::string> sx("x"), sy("y");

  // Node x
  sx.add("apple");
  sx.rmv("apple");
  // Node y
  sy.add("juice");
  sy.add("apple");

  // Join into one object and show it
  sx.join(sy);
  std::cout << sx.read() << std::endl;
}

void example2()
{
  dtcrdt::rworset<std::string, char> sx('x'), sy('y');

  // Node x
  sx.add("apple");
  sx.rmv("apple");
  // Node y
  sy.add("juice");
  sy.add("apple");

  // Join into one object and show it
  sx.join(sy);
  std::cout << sx.read() << std::endl;
}

void example3()
{
  dtcrdt::gset<int> sx;

  // Node x does initial operations
  sx.add(1);
  sx.add(4);

  // Replicate full state in sy;
  dtcrdt::gset<int> sy = sx;

  // Node y records operations in delta
  dtcrdt::gset<int> dy;
  dy = sy.add(2);
  dy.join(sy.add(3)); // Join delta to delta

  std::cout << sy.read() << std::endl; // ( 1 2 3 4 )

  // Merge deltas ( 2 3 ) to node x
  std::cout << dy.read() << std::endl; // ( 2 3 )
  std::cout << sx.read() << std::endl; // ( 1 4 )
  sx.join(dy);
  std::cout << sx.read() << std::endl; // ( 1 2 3 4 )
}

void test_maxpairs()
{
  std::cout << "--- Testing: dtcrdt::lexjoin on pairs --\n";
  std::pair<int, dtcrdt::gset<int>> a, b, c, d;
  a.first = 1;
  a.second.add(0);
  b.first = 0;
  b.second.add(1);
  c = dtcrdt::join(a, b);
  std::cout << c << std::endl;
  d = dtcrdt::lexjoin(a, b);
  std::cout << d << std::endl;
  std::pair<float, dtcrdt::twopset<char>> e;
}

void test_lwwreg()
{
  std::cout << "--- Testing: lwwreg --\n";
  dtcrdt::lwwreg<int, std::string> r;

  r.write(1, "Hello");
  std::cout << r << std::endl;
  r.write(0, "My");
  std::cout << r << std::endl;
  r.write(3, "World");

  std::cout << r << std::endl;
  std::cout << r.write(2, "a") << std::endl;
  std::cout << r.read() << std::endl;
}

void test_rwlwwset()
{
  std::cout << "--- Testing: rwlwwset --\n";
  dtcrdt::rwlwwset<int, std::string> s;
  s.add(1, "a");
  s.add(1, "b");
  s.add(10000, "e");
  s.add(2, "b");
  std::cout << s << std::endl;
  std::cout << s.in("b") << std::endl;
  dtcrdt::rwlwwset<int, std::string> t;
  t.rmv(2, "b");
  t.rmv(6, "e");
  t.add(1, "c");
  s.join(t);
  std::cout << s.in("b") << std::endl;
  std::cout << s << std::endl;
}

void test_ewflag()
{
  std::cout << "--- Testing: ewflag --\n";
  dtcrdt::ewflag<> o1("id x"), o2("id y"), do1, do2;

  do1.join(o1.enable());

  do2.join(o2.enable());
  do2.join(o2.enable()); // re-enable is fine

  dtcrdt::ewflag<> o3 = dtcrdt::join(o1, o2);
  dtcrdt::ewflag<> o4 = dtcrdt::join(join(o1, do1), dtcrdt::join(o2, do1));
  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;
  std::cout << o4.read() << std::endl;
  o3.disable();
  o4.join(o3);
  std::cout << o4 << std::endl;
  std::cout << o4.read() << std::endl;
}

void test_dwflag()
{
  std::cout << "--- Testing: dwflag --\n";
  dtcrdt::dwflag<> o1("id x"), o2("id y"), do1, do2;

  do1.join(o1.disable());

  do2.join(o2.disable());
  do2.join(o2.disable()); // re-disable is fine

  dtcrdt::dwflag<> o3 = dtcrdt::join(o1, o2);
  dtcrdt::dwflag<> o4 = dtcrdt::join(join(o1, do1), dtcrdt::join(o2, do1));
  std::cout << o3 << std::endl;
  std::cout << o4 << std::endl;
  std::cout << o4.read() << std::endl;
  o3.enable();
  o4.join(o3);
  std::cout << o4 << std::endl;
  std::cout << o4.read() << std::endl;
}

void test_ormap()
{
  dtcrdt::ormap<std::string, dtcrdt::twopset<std::string>> m1, m2;
  m1["color"].add("red");
  m1["color"].add("blue");
  m2["taste"].add("bitter");
  m2["color"].add("green");
  std::cout << m2["taste"] << std::endl;
  m1.join(m2);
  std::cout << m1["color"] << std::endl;
  m1.erase("taste");
  std::cout << m1["taste"] << std::endl;

  dtcrdt::dotcontext<std::string> dc;
  dtcrdt::aworset<int> s1("x", dc), s2("x", dc);
  s1.add(1);
  s2.add(2);
  std::cout << s1 << std::endl;
  std::cout << s2 << std::endl;

  dtcrdt::dotcontext<std::string> dc2;
  dtcrdt::ormap<std::string, dtcrdt::aworset<std::string>> m3("x", dc), m4("y", dc2);
  m3["color"].add("red");
  m3["color"].add("blue");
  m4["color"].add("green");
  std::cout << m3["color"] << std::endl;
  std::cout << m4["color"] << std::endl;
  m3.join(m4);
  std::cout << m3["color"] << std::endl;
  m3["color"].rmv("green");
  m3.join(m4);
  std::cout << m3["color"] << std::endl;

  dtcrdt::ormap<std::string, dtcrdt::aworset<std::string>> mx("x"), d1, d2;
  mx["color"].add("red");
  mx["color"].add("blue");

  // Now make some deltas, d1 and d2

  d1 = mx.erase("color");

  d2["color"].join(mx["color"].add("black"));

  std::cout << d1 << std::endl; // Will erase observed dots in the "color" entry
  std::cout << d2 << std::endl; // Will add a dot (x:3) tagged "black" entry under "color"

  dtcrdt::ccounter<int> cc1("x"), cc2("y");
  cc1.inc(10);
  cc2.join(cc1);
  cc2.inc(10);
  std::cout << cc1 << std::endl;
  cc1.inc();
  std::cout << cc1 << std::endl;
  cc1.dec();
  std::cout << cc1 << std::endl;
  cc1.reset();
  std::cout << cc1 << std::endl;
  std::cout << cc1.read() << std::endl;
  cc1.inc(5);
  cc1.join(cc2);
  std::cout << cc1 << std::endl;
  std::cout << cc1.read() << std::endl;

  std::cout << "--- Map I ---" << std::endl;
  dtcrdt::ormap<std::string, dtcrdt::rworset<std::string>> m5("x"), m6("y");
  m5["color"].add("red");
  m5["taste"].add("bitter");
  m6["sound"].add("loud");
  m6["color"].add("blue");
  std::cout << "m5 " << m5 << std::endl;
  std::cout << "m6 " << m6 << std::endl;
  m5.join(m6);
  std::cout << "m5 " << m5 << std::endl;
  m6.erase("sound");
  std::cout << "m6 " << m6 << std::endl;
  m5.join(m6);
  std::cout << "m5 " << m5 << std::endl;
  std::cout << m5.erase("color");
  std::cout << m5.reset();
  std::cout << "m5 " << m5 << std::endl;
  m5.join(m6);
  std::cout << "m5 " << m5 << std::endl;

  std::cout << "--- Map F ---" << std::endl;

  dtcrdt::ormap<int, dtcrdt::ormap<std::string, dtcrdt::aworset<std::string>>> m7("x");
  m7[2]["color"].add("red");
  std::cout << m7 << std::endl;
}

void benchmark1()
{
  dtcrdt::aworset<int, char> g('i');
  //  dtcrdt::twopset<int> g;

  using namespace std::chrono;

  steady_clock::time_point t1 = steady_clock::now();

  //const long double TimeBefore = time(0);

  for (int i = 1; i < 1000; i++) // 1k
  {
    g.add(i);
  }
  for (int i = 1; i < 1000; i += 2) // 1k
  {
    g.rmv(i);
  }
  for (int i = 999; i > 0; i--) // 1k
  {
    g.add(i);
  }

  steady_clock::time_point t2 = steady_clock::now();

  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  std::cout << "It took me " << time_span.count() << " seconds.";
  std::cout << std::endl;

  std::cout << g.in(0) << std::endl;
  std::cout << g.in(10) << std::endl;

  /*
  const long double TimeAfter = time(0);

  std::cout << "Elapsed System Time in Seconds is " << TimeAfter-TimeBefore << "." << std::endl;
  */
}

void example_gset()
{
  dtcrdt::gset<std::string> a, b;

  a.add("red");
  b.add("blue");

  std::cout << dtcrdt::join(a, b) << std::endl; // GSet: ( blue red )
}

void example_twopset()
{
  dtcrdt::twopset<float> a, b;

  a.add(3.1415);
  a.rmv(3.1415);
  b.add(42);
  b.add(3.1415);

  std::cout << dtcrdt::join(a, b) << std::endl; // 2PSet: S( 42 ) T ( 3.1415 )

  dtcrdt::gset<float> c;
  c.add(42);

  std::cout << (join(a, b).read() == c.read()) << std::endl; // true
}

void example_pair()
{
  std::pair<dtcrdt::gset<int>, dtcrdt::gset<char>> a, b, c;

  a.first.add(0);
  b.first.add(1);
  a.second.add('a');
  b.second.add('x');
  b.second.add('y');

  c = dtcrdt::join(a, b);

  std::cout << c << std::endl; // (GSet: ( 0 1 ),GSet: ( a x y ))
}

void example_lexpair()
{
  std::pair<int, float> lww_a(12, 42), lww_b(20, 3.1415);

  std::cout << dtcrdt::join(lww_a, lww_b) << std::endl;    // (20,42)
  std::cout << dtcrdt::lexjoin(lww_a, lww_b) << std::endl; // (20,3.1415)
}

void example_gcounter()
{
  dtcrdt::gcounter<unsigned int> x("x"), y("y"), z("z");

  x.inc();
  x.inc();
  y.inc(2);
  z.join(x);
  z.join(y);

  std::cout << z.read() << std::endl; // 4

  x.inc(2);
  z.inc(2);
  z.join(x);
  z.join(x);

  std::cout << z.read() << std::endl; // 8
  std::cout << z << std::endl;        // GCounter: ( x->4 y->2 z->2 )
}

void example_pncounter()
{
  dtcrdt::pncounter<int, char> x('a'), y('b');

  x.inc(4);
  x.dec();
  y.dec();

  std::cout << (x.read() == y.read()) << std::endl; // value is diferent

  x.join(y);
  y.join(x);

  std::cout << (x.read() == y.read()) << std::endl; // value is the same, both are 2
}

void example_lexcounter()
{
  dtcrdt::lexcounter<int> x("a"), y("b");

  x.inc(4);
  x.dec();
  y.dec();

  std::cout << (x.read() == y.read()) << std::endl; // value is diferent

  x.join(y);
  y.join(x);

  std::cout << (x.read() == y.read()) << std::endl; // value is the same, both are 2
}

void example_ccounter()
{
  dtcrdt::ccounter<int> x("a"), y("b");

  x.inc(4);
  x.dec();
  y.dec();

  std::cout << (x.read() == y.read()) << std::endl; // value is diferent

  x.join(y);
  y.join(x);

  std::cout << (x.read() == y.read()) << std::endl; // value is the same, both are 2

  x.reset();

  std::cout << x.read() << std::endl; // you guessed correctly, its 0
}

void example_aworset()
{
  dtcrdt::aworset<float> x("a"), y("b");

  x.add(3.14);
  x.add(2.718);
  x.rmv(3.14);
  y.add(3.14);

  x.join(y);

  std::cout << x.read() << std::endl; // Both 3.14 and 2.718 are there

  x.reset();
  x.join(y);

  std::cout << x.read() << std::endl; // Empty, since 3.14 adition from "b" was visible
}

void example_rworset()
{
  dtcrdt::rworset<float> x("a"), y("b");

  x.add(3.14);
  x.add(2.718);
  x.rmv(3.14);
  y.add(3.14);

  x.join(y);

  std::cout << x.read() << std::endl; // Only 2.718 is there, since rmv wins

  x.reset();
  x.join(y);

  std::cout << x.read() << std::endl; // Empty
}

void example_ormap()
{
  dtcrdt::ormap<std::string, dtcrdt::aworset<std::string>> mx("x"), my("y");

  mx["paint"].add("blue");
  mx["sound"].add("loud");
  mx["sound"].add("soft");
  my["paint"].add("red");
  my["number"].add("42");

  mx.join(my);

  std::cout << mx << std::endl; // this map includes all added elements

  my["number"].rmv("42");
  mx.join(my);

  std::cout << mx << std::endl; // number set is now empty also in mx

  mx.erase("paint");
  my["paint"].add("green");

  my.join(mx);

  std::cout << my << std::endl; // in the "paint" key there is only "green"

  dtcrdt::ormap<int, dtcrdt::ormap<std::string, dtcrdt::aworset<std::string>>> ma("alice"), mb("bob");

  ma[23]["color"].add("red at 23");
  ma[44]["color"].add("blue at 44");
  mb[44]["sound"].add("soft at 44");

  ma.join(mb);

  std::cout << ma << std::endl; // Map with two map entries, "44" with two entries
}

void example_gmap()
{
  dtcrdt::gmap<char, int> gmx, gmy;

  gmx['a'] = 1;
  gmx['b'] = 0;
  gmy['a'] = 3;
  gmy['c'] = 0;

  gmx.join(gmy);
  std::cout << gmx << std::endl;
  gmx.join(gmy);
  std::cout << gmx << std::endl;
}

void example_bcounter()
{
  dtcrdt::bcounter<int, char> bcx('a'), bcy('b');

  bcx.inc(10);
  std::cout << bcx << std::endl;
  bcy.inc(3);

  std::cout << bcx.read() << std::endl;
  std::cout << bcy.read() << std::endl;

  bcy.mv(1, 'a');
  bcy.mv(1, 'a');

  bcx.join(bcy);
  std::cout << bcx << std::endl;
  std::cout << bcx.read() << std::endl;
  std::cout << bcx.local() << std::endl;
  std::cout << bcy << std::endl;
  std::cout << bcy.read() << std::endl;
  std::cout << bcy.local() << std::endl;

  bcx.mv(10, 'b');
  std::cout << bcx << std::endl;
  std::cout << bcx.read() << std::endl;
  std::cout << bcx.local() << std::endl;
}

void example_orseq()
{
  std::vector<bool> bl, br;
  bl.push_back(false);
  bl.push_back(true);
  bl.push_back(false);
  bl.push_back(true);
  //  bl.push_back(true);
  //  bl.push_back(false);
  //  bl.push_back(true);
  br.push_back(false);
  br.push_back(true);
  br.push_back(true);
  //  br.push_back(false);
  br.push_back(true);

  std::cout << bl << std::endl;
  std::cout << br << std::endl;
  std::cout << "size " << bl.size() << std::endl;

  std::cout << (bl < br) << std::endl;
  std::cout << dtcrdt::among(bl, br) << std::endl;

  // Simple ORSEQ

  dtcrdt::orseq<> seq("rid");
  seq.push_back('a');
  std::cout << seq << std::endl;
  seq.push_back('b');
  std::cout << seq << std::endl;
  seq.push_back('c');
  seq.push_front('0');
  seq.push_front('1');
  std::cout << seq << std::endl;

  auto i = seq.begin();
  i++;
  seq.insert(i, 'x');
  std::cout << seq << std::endl;

  dtcrdt::orseq<> seq2("b");
  seq2.push_back('y');
  std::cout << seq2 << std::endl;

  seq.join(seq2);
  std::cout << seq << std::endl;
  seq2.erase(seq2.begin());
  seq.join(seq2);
  std::cout << seq << std::endl;
  seq.reset();
  std::cout << seq << std::endl;

  // Map with a ORSEQ inside

  dtcrdt::ormap<std::string, dtcrdt::orseq<char>> ms1("id1"), ms2("id2");
  ms1["upper"].push_back('a');
  ms2["upper"].push_front('b');
  ms2["lower"].push_front('c');
  ms1.join(ms2);
  std::cout << ms1 << std::endl;
  ms2.erase("upper");
  ms1.join(ms2);
  std::cout << ms1 << std::endl;

  // Metadata growth, insertions and deletions of added elements,
  // while keeping the first element there

  dtcrdt::orseq<> seq3("s3");
  seq3.push_back('a');
  std::cout << seq3 << std::endl;
  for (int ops = 0; ops < 1000; ops++)
  {
    seq3.push_front('d');
    seq3.erase(seq3.begin());
  }
  std::cout << seq3 << std::endl;

  // Metadata growth, insertions and deletions of added elements,
  // while keeping the last added element there

  dtcrdt::orseq<> seq4("s4");
  seq4.push_back('a');
  std::cout << seq4 << std::endl;
  for (int ops = 0; ops < 1000; ops++)
  {
    seq4.push_back('d');
    seq4.erase(seq4.begin());
  }
  std::cout << seq4 << std::endl;
  seq4.erase(seq4.begin());
  std::cout << seq4 << std::endl;
}

void example_mvreg()
{
  dtcrdt::mvreg<std::string> x("uid-x"), y("uid-y");

  x.write("hello");
  x.write("world");

  y.write("world");
  y.write("hello");

  y.join(x);

  std::cout << y.read() << std::endl; // Output is ( hello world )

  y.write("mars");
  x.join(y);

  std::cout << x.read() << std::endl; // Output is ( mars )

  dtcrdt::mvreg<int> a("uid-a"), b("uid-b");

  a.write(0);
  b.write(3);
  a.join(b);

  std::cout << a.read() << std::endl; // Output is ( 0 3 )

  a.resolve();

  std::cout << a.read() << std::endl; // Output is ( 3 )

  a.write(1); // Value can go down again

  std::cout << a.read() << std::endl; // Output is ( 1 )

  dtcrdt::mvreg<std::pair<int, int>> j("uid-j"), k("uid-k"), l("uid-l");

  j.write(std::pair<int, int>(0, 0));
  k.write(std::pair<int, int>(1, 0));
  l.write(std::pair<int, int>(0, 1));

  j.join(k);
  j.join(l);
  j.resolve();

  std::cout << j.read() << std::endl; // Output is ( (0,1) (1,0) )
}

void test_bag()
{
  dtcrdt::bag<std::pair<int, int>> b("i");
  dtcrdt::bag<std::pair<int, int>> c("j");

  b.mydata().first = 1;
  std::cout << b.mydata() << std::endl;
  std::cout << b << std::endl;
  c.join(b);
  b.mydata().first = 3;
  b.join(c);
  std::cout << b << std::endl;
  c.reset();
  b.join(c);
  std::cout << b << std::endl;

  // Now inside a map
  dtcrdt::ormap<std::string, dtcrdt::bag<std::pair<int, int>>> ma("y");

  std::cout << ma["a"] << std::endl;
  ma["a"].fresh();
  std::cout << ma["a"] << std::endl;
  std::cout << ma["a"].mydata() << std::endl;
  ma["a"].mydata().first += 1;
  std::cout << ma["a"].mydata() << std::endl;
  std::cout << ma << std::endl;
}

void test_rwcounter()
{
  dtcrdt::rwcounter<int> rwc1("i"), rwc2("j");

  rwc1.inc();
  rwc1.inc(2);
  rwc1.dec();
  rwc2.inc(5);
  std::cout << rwc1 << std::endl;
  std::cout << rwc2 << std::endl;
  rwc1.join(rwc2);
  std::cout << rwc1 << std::endl;
  std::cout << rwc1.read() << std::endl;
  std::cout << "Reset:" << rwc2.reset() << std::endl;
  std::cout << "Delta:" << rwc2.inc(1) << std::endl;
  rwc1.join(rwc2);
  std::cout << rwc1 << std::endl;
  std::cout << rwc1.read() << std::endl;
  rwc2.join(rwc1);
  rwc2.reset();
  rwc1.fresh();
  std::cout << rwc1 << std::endl;
  rwc1.inc();
  std::cout << rwc1 << std::endl;
  rwc1.join(rwc2);
  std::cout << rwc1 << std::endl;
  std::cout << rwc1.read() << std::endl;

  dtcrdt::ormap<std::string, dtcrdt::rwcounter<float>> mx("x");

  std::cout << mx["adds"] << std::endl;
  std::cout << mx["adds"] << std::endl;
  std::cout << mx["adds"].inc() << std::endl;
  mx["prints"].inc(5);
  std::cout << "Delta:" << mx["prints"].inc(6) << std::endl;
  mx["adds"].inc();

  std::cout << mx["adds"] << std::endl;
  std::cout << mx["adds"].read() << std::endl;
  std::cout << mx << std::endl;

  dtcrdt::ormap<std::string, dtcrdt::rwcounter<float>> my("y");

  my.join(mx);
  my.erase("prints");
  mx["prints"].fresh(); // without this fresh concurrent inc is lost
  mx["prints"].inc(5);
  mx.join(my);
  std::cout << mx << std::endl;
}

int main(int argc, char *argv[])
{
  test_gset();
  test_twopset();
  test_gcounter();
  test_pncounter();
  test_lexcounter();
  test_aworset();
  test_rworset();
  test_mvreg();
  //  test_maxord();
  test_maxpairs();
  test_lwwreg();
  test_rwlwwset();
  test_ewflag();
  test_dwflag();
  test_ormap();
  test_rwlwwset();
  test_bag();
  test_rwcounter();

  example1();
  example2();
  example3();

  example_gset();
  example_twopset();
  example_pair();
  example_lexpair();
  example_gcounter();
  example_pncounter();
  example_lexcounter();
  example_ccounter();
  example_aworset();
  example_rworset();
  example_ormap();
  example_gmap();
  example_bcounter();
  example_orseq();
  example_mvreg();

  dtcrdt::ormap<std::string, dtcrdt::aworset<std::string>> m1("dev1"), m2("dev2");

  m1["friend"].add("alice");
  m2.join(m1);
  m2.erase("friend");
  m1["friend"].add("bob");

  std::cout << dtcrdt::join(m1, m2) << std::endl; // shows: friend -> {bob}

  dtcrdt::ormap<std::string, dtcrdt::rwcounter<int>> c1("dev1"), c2("dev2");

  c1["counter1"].inc(2);
  c2.join(c1);
  c1["counter2"].inc(1);
  c1.erase("counter1");
  c1["counter1"].inc(3);
  c1["counter2"].fresh();
  c1["counter2"].inc(3);

  std::cout << "counter1: " << dtcrdt::join(c1, c2)["counter1"].read() << std::endl;
  std::cout << "counter2: " << dtcrdt::join(c1, c2)["counter2"].read() << std::endl;
}
