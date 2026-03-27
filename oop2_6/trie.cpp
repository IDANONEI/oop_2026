#include "trie.h"

#include <queue>

TrieNode* CreateNode(char letter)
{
    TrieNode* node = new TrieNode;
    node->letter = letter;
    return node;
}

bool AddKey(TrieNode* root, const std::string& key, const std::string& value)
{
    if (root == nullptr)
    {
        return false;
    }
    TrieNode* current = root;
    for (char ch : key)
    {
        if (current->next.find(ch) == current->next.end())
        {
            current->next[ch] = CreateNode(ch);
        }
        current = current->next[ch];
    }
    if (current->isKey)
    {
        return false;
    }
    current->isKey = true;
    current->key = key;
    current->value = value;
    return true;
}

TrieNode* BuildTrie(const std::map<std::string, std::string>& params)
{
    TrieNode* root = CreateNode('\0');
    for (const auto& [key, value] : params)
    {
        if (key.empty())
        {
            continue;
        }
        if (!AddKey(root, key, value))
        {
            DeleteTrie(root);
            return nullptr;
        }
    }
    BuildFailureLinks(root);
    return root;
}

void BuildFailureLinks(TrieNode* root)
{
    if (root == nullptr)
    {
        return;
    }
    root->fail = root;
    std::queue<TrieNode*> q;
    for (auto& [ch, child] : root->next)
    {
        child->fail = root;
        q.push(child);
    }

    while (!q.empty())
    {
        TrieNode* current = q.front();
        q.pop();

        for (auto& [ch, child] : current->next)
        {
            TrieNode* failNode = current->fail;

            while (failNode != root && failNode->next.find(ch) == failNode->next.end())
            {
                failNode = failNode->fail;
            }

            auto it = failNode->next.find(ch);
            if (it != failNode->next.end() && it->second != child)
            {
                child->fail = it->second;
            }
            else
            {
                child->fail = root;
            }

            q.push(child);
        }
    }
}

void DeleteTrie(TrieNode* root)
{
    if (root == nullptr)
    {
        return;
    }
    for (auto& [ch, child] : root->next)
    {
        DeleteTrie(child);
    }
    delete root;
}