/**
 * @version: 1.1
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Windows.Forms;

namespace gef
{
    internal sealed class DataFile
    {
        public sealed class DataItem
        {
            public enum DATA_ITEM_TYPE
            {
                DATA_ITEM_TYPE_ROOT,
                DATA_ITEM_TYPE_NODE,
                DATA_ITEM_TYPE_ITEM
            }

            private DATA_ITEM_TYPE type;
            public DATA_ITEM_TYPE Type
            {
                get { return type; }
                set { type = value; }
            }

            private string name;
            public string Name
            {
                get { return name; }
                set { name = value; }
            }

            private string value;
            public string Value
            {
                get { return value; }
                set { this.value = value; }
            }

            private DataItem parent = null;
            public DataItem Parent
            {
                get { return parent; }
            }

            private Dictionary<string, DataItem> children = null;
            public Dictionary<string, DataItem> Children
            {
                get { return children; }
            }

            public DataItem(DataItem par)
            {
                children = new Dictionary<string, DataItem>();
                parent = par;
            }

            public DataItem(DataItem par, DATA_ITEM_TYPE _type, string _name, string _value)
            {
                children = new Dictionary<string, DataItem>();
                type = _type;
                name = _name;
                value = _value;
                parent = par;
            }

            public DataItem Peek(string key)
            {
                if (children.ContainsKey(key))
                    return children[key];
                else
                    return null;
            }

            public DataItem this[string key]
            {
                get
                {
                    if (string.IsNullOrEmpty(key))
                        return null;
                    if (!children.ContainsKey(key))
                    {
                        children[key] = new DataItem(this);
                    }

                    return children[key];
                }
                set
                {
                    children[key] = value;
                }
            }

            public void LoadXML(XmlNode node)
            {
                if(node.Name.Equals("node"))
                {
                    type = DATA_ITEM_TYPE.DATA_ITEM_TYPE_NODE;
                }
                else if(node.Name.Equals("item"))
                {
                    type = DATA_ITEM_TYPE.DATA_ITEM_TYPE_ITEM;
                }
                else if(node.Name.Equals("config"))
                {
                    type = DATA_ITEM_TYPE.DATA_ITEM_TYPE_ROOT;
                }
                name = node.Attributes["name"].Value;
                value = node.Attributes["value"].Value;
                foreach (XmlNode sub in node.ChildNodes)
                {
                    string _sub_name = sub.Attributes["name"].Value;
                    children[_sub_name] = new DataItem(this);
                    children[_sub_name].LoadXML(sub);
                }
            }

            public void Save(ref string xml)
            {
                string ident = string.Empty;
                if (DATA_ITEM_TYPE.DATA_ITEM_TYPE_ROOT == type)
                {
                    ident = "config";
                }
                else if (DATA_ITEM_TYPE.DATA_ITEM_TYPE_NODE == type)
                {
                    ident = "node";
                }
                else if (DATA_ITEM_TYPE.DATA_ITEM_TYPE_ITEM == type)
                {
                    ident = "item";

                }

                xml += "<" + ident + " name=\"" + name + "\" value=\"" + value + "\">";
                foreach (DataItem child in children.Values)
                {
                    child.Save(ref xml);
                }
                xml += "</" + ident + ">";
            }

            public Int32 ToInt32()
            {
                return Int32.Parse(value);
            }

            public Int16 ToInt16()
            {
                return Int16.Parse(value);
            }

            public Int64 ToInt64()
            {
                return Int64.Parse(value);
            }

            public Single ToSingle()
            {
                return Single.Parse(value);
            }

            public Double ToDouble()
            {
                return Double.Parse(value);
            }

            public void Set(object obj)
            {
                if(name.Equals(string.Empty))
                {
                    throw new Exception("Invalid configuration name");
                }
                if(typeof(string).Equals(obj.GetType()))
                {
                    value = (string)obj;
                }
                else
                {
                    value = obj.ToString();
                }
            }

            public void Set(string key, object obj)
            {
                name = key;
                Set(obj);
            }
        }

        private const string _version = "1.0.0";

        private XmlDocument document = null;

        private DataItem item = null;
        public DataItem Item
        {
            get { return item; }
            set { item = value; }
        }

        public DataFile()
        {
            document = new XmlDocument();
            item = new DataItem(null, DataItem.DATA_ITEM_TYPE.DATA_ITEM_TYPE_ROOT, "version", _version);
        }

        public bool Load(string file)
        {
            try
            {
                document.Load(file);
                item.LoadXML(document.FirstChild);
            }
            catch (Exception ex)
            {
                ShellUtil.MsgBox(ex.Message, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

        public bool Save(string file)
        {
            string xml = string.Empty;
            try
            {
                item.Save(ref xml);
                document.LoadXml(xml);
                document.Save(file);
            }
            catch (Exception ex)
            {
                ShellUtil.MsgBox(ex.Message, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

        public DataItem PeekItem(string path, bool create)
        {
            string[] p = path.Split('.');
            DataFile.DataItem _item = Item.Peek(p[0]);
            if (_item == null)
            {
                if (create)
                {
                    _item = new DataItem
                    (
                        Item,
                        p.Length == 0 ? DataItem.DATA_ITEM_TYPE.DATA_ITEM_TYPE_ITEM : DataItem.DATA_ITEM_TYPE.DATA_ITEM_TYPE_NODE,
                        p[0],
                        string.Empty
                    );
                    Item[_item.Name] = _item;
                }
                else
                {
                    return null;
                }
            }
            for (int i = 1; i < p.Length; i++)
            {
                if (_item.Children.ContainsKey(p[i]))
                {
                    _item = _item.Children[p[i]];
                }
                else
                {
                    if (create)
                    {
                        DataItem it = new DataItem
                        (
                            _item,
                            i == p.Length - 1 ? DataItem.DATA_ITEM_TYPE.DATA_ITEM_TYPE_ITEM : DataItem.DATA_ITEM_TYPE.DATA_ITEM_TYPE_NODE,
                            p[i],
                            string.Empty
                        );
                        _item.Children[p[i]] = it;
                        _item = it;
                    }
                    else
                    {
                        return null;
                    }
                }
            }

            return _item;
        }
    }
}
