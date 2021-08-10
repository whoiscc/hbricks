#include <cstring>

#include <gtest/gtest.h>

extern "C" {
#include "StrBuf.h"
#include "Polyfill.h"
}

class StrBufTest : public testing::Test {
protected:
  uint8_t mem[SIZEOF_StrBufState]{};
  StrBufState *str_buf{};

  void SetUp() override {
    str_buf = InitStrBuf(mem);
  }
  void TearDown() override {
    DropStrBuf(str_buf);
  }
};

TEST_F(StrBufTest, CanInit) {  // NOLINT
}

TEST_F(StrBufTest, SmallAppendView) {  // NOLINT
  const char *s = "Hello, world!";
  StrBuf.Append(str_buf, s);
  ASSERT_STREQ(StrBuf.View(str_buf), s);
}

TEST_F(StrBufTest, LargeAppendView) {  // NOLINT
  // https://zh.moegirl.org.cn/index.php?title=%E5%98%89%E7%84%B6%E5%B0%8F%E5%A7%90%E7%9A%84%E7%8B%97
  const char *s = R"EOF(
I want to be Miss Diana's dog.
But Miss Diana said that she likes cats, and I cried. I know why I cry. Cause neither a dog nor a cat, I am a mouse. I never expected Miss Diana to like me. I understand that everyone likes dogs or cats who are outstanding, smart, talented, rich and adorable. No one likes disgusting and dirty mice.
But I still asked Miss Diana: "Can I be your dog?"
I know I can't. But if she likes dogs, I can always watch her by her side, even if in her arms is always a REAL dog.
But she said that she likes cats.
She was still looking at me now, still making me laugh because the cat hasn't appeared yet.
And only me, the mouse, crawls out of the hole every day, looking at her from a distance.
When the cat she likes comes, I should roll back to my hole again.
But I still like her. May she please look at me more when I am around?
Miss Diana said that she would spend every Christmas Eve with us. I don't know who are those “us”. But I wish I would be included.
Cats were still afraid of Miss Diana.
I would attract cats for her.
I know that, if not being careful enough I will die. I know that.
At that time, Miss Diana would probably package my body and throw me out the door.
Then I become a pack of rat chips, lol.
I hope she could throw me closer because I would still like her.
For ever.
My soul looked inside through the window, the bells rang softly, Miss Diana leaned back on the sofa lazily, and an orange cat sat on her shoulder, pretending to be docile. Her face was enlightened by the dancing flames in the fireplace, and my frozen heart was slightly warmed by the north wind.
)EOF";
  StrBuf.Append(str_buf, s);
  ASSERT_STREQ(StrBuf.View(str_buf), s);
}

TEST_F(StrBufTest, Incremental) {  // NOLINT
  char expect[64 + 1];
  std::memset(expect, 0, sizeof(expect));
  for (int i = 0; i < 64; i += 1) {
    char c = (char) ('0' + (i % 10));
    char s[2] = {c, '\0'};
    StrBuf.Append(str_buf, s);
    expect[i] = c;
    ASSERT_STREQ(StrBuf.View(str_buf), expect);
  }
}

TEST_F(StrBufTest, Format) {  // NOLINT
  StrBuf.FmtWrite(str_buf, "ans = %d", 42);
  ASSERT_STREQ(StrBuf.View(str_buf), "ans = 42");
}

TEST_F(StrBufTest, Repr) {  // NOLINT
  StrBuf.Append(str_buf, "Hello!");
  StrBuf.Ex.Repr(str_buf, str_buf);
  ASSERT_STREQ(StrBuf.View(str_buf),
               "Hello!"
               "StrBuf[emb]{len=6,view=Hello!}");
  for (int i = 0; i < 64 - 1; i += 1) {
    StrBuf.Ex.Repr(str_buf, str_buf);
  }

  const char *v = StrBuf.View(str_buf);
  ASSERT_STREQ(&v[std::strlen(v) - 4], "...}");

  const char *s = "StrBuf";
  int count = 0;
  while ((v = std::strstr(v, s))) {
    count += 1;
    v += std::strlen(s);
  }
  ASSERT_EQ(count, 64);
}
