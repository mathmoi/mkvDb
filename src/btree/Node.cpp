#include "mkvdb/btree/Node.hpp"

#include "mkvdb/common/MkvDBException.hpp"

#include <algorithm>
#include <cassert>
#include <compare>

namespace mkvdb::btree
{
    Node::Node(const BTree& btree,
               std::shared_ptr<pager::Page> page,
               common::KeyValueSize value_size,
               bool initialize)
    : page_(page),
      key_size_(btree.key_size()),
      value_size_(value_size)
    {
        if(initialize)
        {
            items_count(0);
        }
    }

    void Node::Insert(common::ConstByteSpan key, common::ConstByteSpan value)
    {
        auto position = SearchIndex(key);
        if(position.second)
        {
            throw common::MkvDBException("Cannot insert, the key already exists.");
        }

        auto to_move = items_array_span().subspan(position.first * item_size());

        std::copy_backward(to_move.begin(), to_move.end(), to_move.end() + item_size());

        auto key_target   = to_move.subspan(0, key_size_);
        auto value_target = to_move.subspan(key_size_, value_size_);
        std::copy(key.begin(), key.end(), key_target.begin());
        std::copy(value.begin(), value.end(), value_target.begin());

        items_count(items_count() + 1);
    }

    bool Node::CheckForSpace() const
    {
        return item_size() <= available_space();
    }

    common::FileSize Node::header_size() const
    {
        return ITEMS_COUNT_SIZE;
    }

    std::pair<Node::ItemIndex, bool> Node::SearchIndex(common::ConstByteSpan key)
    {
        std::pair<Node::ItemIndex, bool> result(0, false);

        auto left  = 0;
        auto right = items_count() - 1;

        while(left <= right)
        {
            auto mid     = left + (right - left) / 2;
            auto mid_key = GetItemKey(mid);
            auto order   = std::lexicographical_compare_three_way(mid_key.begin(),
                                                                mid_key.end(),
                                                                key.begin(),
                                                                key.end());

            if(order == std::strong_ordering::equal)
            {
                result.first  = mid;
                result.second = true;
                right         = mid - 1;
            }
            else if(order == std::strong_ordering::less)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }

        // If the key was not found, 'left' will be the position were the key would have
        // been found.
        if(!result.second)
        {
            result.first = left;
        }

        return result;
    }

} // namespace mkvdb::btree