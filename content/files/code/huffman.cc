#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;

struct HuffCode {
  uint8_t val;
  uint32_t freq;
  string code;

  HuffCode(uint8_t v = ' ', uint32_t f = 1) : val(v), freq(f) {}
};

enum HuffType { INTERNAL, LEAF };

struct HuffNode {
  HuffCode data;
  HuffNode *left, *right;
  HuffType type;

  HuffNode(HuffCode &&d = HuffCode(), 
          HuffNode *l = nullptr, HuffNode *r = nullptr, 
          HuffType t = LEAF)
      : data(d), left(l), right(r), type(t) {}

  class Compare {
  public:
    bool operator()(HuffNode *a, HuffNode *b) {
      return a->data.freq > b->data.freq;
    }
  };
};

struct HuffTree {
  HuffNode *root;
  uint32_t size;

  HuffTree() : root(nullptr), size(0) {}

  // for each leaf node, give a specific code
  // left: 0, right: 1
  void _encode_tree(HuffNode *p, string &prefix, uint8_t t) {
    if (p == nullptr) return;
    // skip the root node
    if (p != root) {
      p->data.code = prefix;
      p->data.code += (t == 0 ? "0" : "1");
      if (p->type == LEAF) return;
    }
    _encode_tree(p->left, p->data.code, 0);
    _encode_tree(p->right, p->data.code, 1);
  }
  void encode_tree() {
    _encode_tree(root, root->data.code, 0);
  }

  // for debug
  void __print_tree(HuffNode *p, int prefix) {
    char prefix_str[prefix + 1];
    for (int i = 0; i < prefix; i++) prefix_str[i] = ' ';
    prefix_str[prefix] = '\0';
    if (p == nullptr) return;
    printf("%s--", prefix_str);
    if (p->type == LEAF) {
      if (p->data.code == "") {
        printf("'%d'\n", p->data.val);
      } else {
        printf("[%s]'%d'\n", p->data.code.c_str(), p->data.val);
      }
    } else {
      printf("%u:\n", p->data.freq);
    }
    __print_tree(p->left, prefix + 2);
    __print_tree(p->right, prefix + 2);
  }
  void _print_tree() {
    printf("huffman tree(%u)\n", size);
    __print_tree(root, 0);
  }
};

void build_tree(string &src, HuffTree &t) {
  // loop and count the freq of char
  map<uint8_t, HuffNode *> m;
  for (uint8_t ch: src) {
    if (m.find(ch) == m.end()) {
      HuffCode code(ch);
      HuffNode *node = new HuffNode(move(code));
      m.insert({ch, node});
    } else {
      m[ch]->data.freq++;
    }
  }
  // push all into priority queue
  priority_queue<HuffNode *, vector<HuffNode *>, HuffNode::Compare> q;
  for (auto &pair: m) q.push(pair.second);
  // get node nums
  uint32_t size = q.size();
  // build huffman tree
  while (q.size() >= 2) {
    HuffNode *left = q.top();
    q.pop();
    HuffNode *right = q.top();
    q.pop();
    HuffCode code(' ', left->data.freq + right->data.freq);
    HuffNode *parent = new HuffNode(move(code), left, right, INTERNAL);
    q.push(parent);
  }
  assert(q.size() == 1);
  t.root = q.top();
  t.size = size;
  t.encode_tree();
}

void encode(string &src, string &dst, HuffTree &t) {
  build_tree(src, t);
  map<uint8_t, HuffNode *> m;
  HuffNode *p = t.root;
  queue<HuffNode *> q;
  q.push(p);
  while (!q.empty()) {
    p = q.front();
    q.pop();
    if (p->type == LEAF) m.insert({p->data.val, p});
    if (p->left != nullptr) q.push(p->left);
    if (p->right != nullptr) q.push(p->right);
  }
  for (uint8_t ch: src) {
    dst.append(m[ch]->data.code);
  }
}

void decode(string &src, string &dst, HuffTree &t) {
  HuffNode *p = t.root;
  for (uint8_t ch: src) {
    p = (ch == '0') ? p->left : p->right;
    if (p->type == LEAF) {
      dst.push_back(p->data.val);
      p = t.root;
    }
  }
}

/**
 * linux style I/O
 */
void read_file(const char *filename, string &contents) {
  int fd = open(filename, O_RDONLY);
  if (fd == -1) return;
  struct stat st;
  stat(filename, &st);
  int size = st.st_size;
  char *buffer = new char[size];
  read(fd, buffer, size);
  contents = string(buffer);
}

/**
 * linux style I/O
 */
void write_file(const char *filename, string &contents, HuffTree &t) {
  int fd = open(filename, O_WRONLY | O_CREAT);
  if (fd == -1) return;
  int size = contents.size();
  write(fd, contents.c_str(), size);
}

int main(int argc, char *argv[]) {
  assert(argc == 3);
  char *src_file = argv[1];
  char *dst_file = argv[2];

  string src;
  read_file(src_file, src);

  string dst;
  HuffTree t;
  encode(src, dst, t);
  write_file(dst_file, dst, t);

  string decoded;
  decode(dst, decoded, t);
  return 0;
}
