#include <lib/ArgParser.h>
#include <gtest/gtest.h>
#include <sstream>


using namespace ArgumentParser;

/*
    Функция принимает в качество аргумента строку, разделяет ее по "пробелу"
    и возвращает вектор полученных слов
*/
std::vector<std::string> SplitString(const std::string& str) {
    std::istringstream iss(str);

    return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}

TEST(ArgParserTestSuite, MyTestInt) {
    ArgParser parser("My Parser");
    std::vector<int> a;
    parser.AddIntArgument("param1").MultiValue(2).StoreValues(a).Default(1);
    ASSERT_TRUE(parser.Parse(SplitString("app")));
    ASSERT_EQ(parser.GetIntValue("param1", 0), 1);
    ASSERT_EQ(a[0], 1);
}

TEST(ArgParserTestSuite, MyTestString) {
    ArgParser parser("My Parser");
    std::vector<std::string> a;
    parser.AddStringArgument('p',"param1", "def").MultiValue(2).StoreValues(a).Default("123");
    ASSERT_TRUE(parser.Parse(SplitString("app")));
    ASSERT_EQ(parser.GetStringValue('p', 0), "123");
    ASSERT_EQ(a[0], "123");
}

TEST(ArgParserTestSuite, MyTestFlag) {
    ArgParser parser("My Parser");
    bool a;
    parser.AddFlag('p',"param1", "def").StoreValue(a).Default(true);
    ASSERT_TRUE(parser.Parse(SplitString("app")));
    ASSERT_EQ(parser.GetFlag('p'), true);
    ASSERT_EQ(a, true);
}

TEST(ArgParserTestSuite, MyTestHelp) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddIntArgument('p', "param1", "def").Positional().MultiValue(2).Default(1);
    parser.AddStringArgument('f',"param2", "def").Default("123");
    parser.AddFlag("param3", "def").Default(true);
    parser.Parse(SplitString("app"));
    std::cout << parser.HelpDescription();
    ASSERT_TRUE(parser.Parse(SplitString("app")));
}

TEST(ArgParserTestSuite, MyTestParser) {
    ArgParser parser("My Parser");
    parser.AddIntArgument('p',"param1").MultiValue(2).Default(1);
    ASSERT_TRUE(parser.Parse(SplitString("app -p=2 -p=3")));
    ASSERT_EQ(parser.GetIntValue("param1", 0), 2);
    ASSERT_EQ(parser.GetIntValue("param1", 1), 3);
}

TEST(ArgParserTestSuite, EmptyTest) {
    ArgParser parser("My Empty Parser");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
}


TEST(ArgParserTestSuite, StringTest) {
  ArgParser parser("My Parser");
  parser.AddStringArgument("param1");

  ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
  ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, ShortNameTest) {
  ArgParser parser("My Parser");
  parser.AddStringArgument('p', "param1");

  ASSERT_TRUE(parser.Parse(SplitString("app -p=value1")));
  ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, DefaultTest) {
  ArgParser parser("My Parser");
  parser.AddStringArgument("param1").Default("value1");

  ASSERT_TRUE(parser.Parse(SplitString("app")));
  ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, NoDefaultTest) {
  ArgParser parser("My Parser");
  parser.AddStringArgument("param1");

  ASSERT_FALSE(parser.Parse(SplitString("app")));
}


TEST(ArgParserTestSuite, StringStoreValueTest) {
  ArgParser parser("My Parser");
  std::string value;
  parser.AddStringArgument("param1").StoreValue(value);

  ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
  ASSERT_EQ(value, "value1");
}

TEST(ArgParserTestSuite, StringStoreValuesTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> values;
    parser.AddStringArgument("param1").MultiValue(3).StoreValues(values).Default("123");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1 --param1=value2")));
    ASSERT_EQ(values[2], "123");
}


TEST(ArgParserTestSuite, MultiStringTest) {
  ArgParser parser("My Parser");
  std::string value;
  parser.AddStringArgument("param1").StoreValue(value);
  parser.AddStringArgument('a', "param2");

  ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1 --param2=value2")));
  ASSERT_EQ(parser.GetStringValue("param2"), "value2");
}


TEST(ArgParserTestSuite, IntTest) {
  ArgParser parser("My Parser");
  parser.AddIntArgument("param1");

  ASSERT_TRUE(parser.Parse(SplitString("app --param1=100500")));
  ASSERT_EQ(parser.GetIntValue("param1"), 100500);
}


TEST(ArgParserTestSuite, MultiValueTest) {
  ArgParser parser("My Parser");
  std::vector<int> int_values;
  parser.AddIntArgument('p', "param1").MultiValue().StoreValues(int_values);

  ASSERT_TRUE(parser.Parse(SplitString("app --param1=1 --param1=2 --param1=3")));
  ASSERT_EQ(parser.GetIntValue("param1", 0), 1);
  ASSERT_EQ(int_values[1], 2);
  ASSERT_EQ(int_values[2], 3);
}


TEST(ArgParserTestSuite, MinCountMultiValueTest) {
  ArgParser parser("My Parser");
  std::vector<int> int_values;
  size_t MinArgsCount = 10;
  parser.AddIntArgument('p', "param1").MultiValue(MinArgsCount).StoreValues(int_values);

  ASSERT_FALSE(parser.Parse(SplitString("app --param1=1 --param1=2 --param1=3")));
}


TEST(ArgParserTestSuite, FlagTest) {
  ArgParser parser("My Parser");
  parser.AddFlag('f', "flag1");

  ASSERT_TRUE(parser.Parse(SplitString("app --flag1")));
  ASSERT_TRUE(parser.GetFlag("flag1"));
}


TEST(ArgParserTestSuite, FlagsTest) {
  ArgParser parser("My Parser");
  bool flag3 ;
  parser.AddFlag('a', "flag1");
  parser.AddFlag('b', "flag2").Default(true);
  parser.AddFlag('c', "flag3").StoreValue(flag3);

  ASSERT_TRUE(parser.Parse(SplitString("app -ac")));
  ASSERT_TRUE(parser.GetFlag("flag1"));
  ASSERT_TRUE(parser.GetFlag("flag2"));
  ASSERT_TRUE(flag3);
}


TEST(ArgParserTestSuite, PositionalArgTest) {
  ArgParser parser("My Parser");
  std::vector<int> values;
  parser.AddIntArgument("Param1").MultiValue(1).Positional().StoreValues(values);

  ASSERT_TRUE(parser.Parse(SplitString("app 1 2 3 4 5")));
  ASSERT_EQ(values[0], 1);
  ASSERT_EQ(values[2], 3);
  ASSERT_EQ(values.size(), 5);
}


TEST(ArgParserTestSuite, HelpTest) {
  ArgParser parser("My Parser");
  parser.AddHelp('h', "help", "Some Description about program");

  ASSERT_TRUE(parser.Parse(SplitString("app --help")));
  ASSERT_TRUE(parser.Help());
}


TEST(ArgParserTestSuite, HelpStringTest) {
  ArgParser parser("My Parser");
  parser.AddHelp('h', "help", "Some Description about program");
  parser.AddStringArgument('i', "input", "File path for input file").MultiValue(1);
  parser.AddFlag('s', "flag1", "Use some logic").Default(true);
  parser.AddFlag('p', "flag2", "Use some logic");
  parser.AddIntArgument("numer", "Some Number");
  parser.Parse(SplitString("app --help"));
  std::cout << parser.HelpDescription();


  ASSERT_TRUE(parser.Parse(SplitString("app --help")));
  // Проверка закоментирована намеренно. Ождиается, что результат вызова функции будет приблизительно такой же,
  // но не с точностью до символа

  // ASSERT_EQ(
  //     parser.HelpDescription(),
  //     "My Parser\n"
  //     "Some Description about program\n"
  //     "\n"
  //     "-i,  --input=<string>,  File path for input file [repeated, min args = 1]\n"
  //     "-s,  --flag1,  Use some logic [default = true]\n"
  //     "-p,  --flag2,  Use some logic\n"
  //     "     --number=<int>,  Some Number\n"
  //     "\n"
  //     "-h, --help Display this help and exit\n"
  // );
}
