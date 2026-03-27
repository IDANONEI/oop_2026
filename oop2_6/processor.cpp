#include "processor.h"
#include <algorithm>

std::string ReadAll(std::istream& in)
{
    std::string text;
    std::string line;
    bool first = true;
    while (std::getline(in, line))
    {
        if (!first)
        {
            text += '\n';
        }
        text += line;
        first = false;
    }
    return text;
}

std::vector<Match> FindMatches(const std::string& text, TrieNode* root)
{
    std::vector<Match> matches;

    if (root == nullptr)
    {
        return matches;
    }

    TrieNode* current = root;

    for (std::size_t i = 0; i < text.size(); ++i)
    {
        char ch = text[i];
        while (current != root && current->next.find(ch) == current->next.end())
        {
            current = current->fail;
        }

        auto nextIt = current->next.find(ch);
        if (nextIt != current->next.end())
        {
            current = nextIt->second;
        }
        else
        {
            current = root;
        }

        TrieNode* temp = current;
        while (temp != root)
        {
            if (temp->isKey)
            {
                const std::size_t keyLength = temp->key.size();

                Match match;
                match.start = i + 1 - keyLength;
                match.end = i;
                match.key = temp->key;
                match.value = temp->value;
                matches.push_back(match);
            }

            temp = temp->fail;
        }
    }

    return matches;
}

std::vector<Match> SelectMatches(const std::vector<Match>& matches)
{
    if (matches.empty())
    {
        return {};
    }

    std::vector<Match> sorted = matches;

    std::sort(sorted.begin(), sorted.end(),
        [](const Match& a, const Match& b)
        {
            if (a.start != b.start)
            {
                return a.start < b.start;
            }

            const std::size_t lenA = a.end - a.start + 1;
            const std::size_t lenB = b.end - b.start + 1;

            if (lenA != lenB)
            {
                return lenA > lenB;
            }
            return a.end < b.end;
        });

    std::vector<Match> selected;
    bool hasCovered = false;
    std::size_t coveredEnd = 0;

    for (const Match& match : sorted)
    {
        if (!hasCovered || match.start > coveredEnd)
        {
            selected.push_back(match);
            coveredEnd = match.end;
            hasCovered = true;
        }
    }

    return selected;
}

std::string ReplaceMatches(const std::string& text, const std::vector<Match>& matches)
{
    if (matches.empty())
    {
        return text;
    }

    std::string result;
    std::size_t pos = 0;

    for (const Match& match : matches)
    {
        while (pos < match.start)
        {
            result += text[pos];
            ++pos;
        }

        result += match.value;
        pos = match.end + 1;
    }

    while (pos < text.size())
    {
        result += text[pos];
        ++pos;
    }

    return result;
}

std::string ProcessText(const std::string& text, TrieNode* root)
{
    const std::vector<Match> matches = FindMatches(text, root);
    const std::vector<Match> selected = SelectMatches(matches);
    return ReplaceMatches(text, selected);
}

std::string ExpandTemplate(const std::string& tpl, const std::map<std::string, std::string>& params)
{
    if (tpl.empty())
    {
        return tpl;
    }
    TrieNode* root = BuildTrie(params);
    if (root == nullptr)
    {
        return tpl;
    }
    const std::string result = ProcessText(tpl, root);
    DeleteTrie(root);

    return result;
}