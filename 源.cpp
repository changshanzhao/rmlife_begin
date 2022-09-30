//*Info类*
Info::Info()//构造函数
{
	//初始化
	memset(name, 0, sizeof(name));
	memset(gender, 0, sizeof(gender));
	memset(phone, 0, sizeof(phone));
	memset(qq, 0, sizeof(qq));
	memset(email, 0, sizeof(email));
	index = 0;
	next = NULL;
}
#ifdef  _VC2010_VER_
ofstream& operator << (ofstream& ofs, const Info& c)//运算符 << 重载
{
	ofs << c.name << " " << c.gender << " " << c.phone << " " << c.qq << " " << c.email << " ";
	return ofs;
}

ifstream& operator >> (ifstream& ifs, Info& c)//运算符 >> 重载
{
	ifs >> c.name >> c.gender >> c.phone >> c.qq >> c.email;
	return ifs;
}
#endif

//*AddressBook类*
AddressBook::AddressBook() //构造函数
{
	head = NULL;
}

AddressBook::~AddressBook() //析构函数
{
	Info* temp = head;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
}

void AddressBook::Load() //*读取文件中的数据*
{
	//文件目录当前目录下的data.txt
	ifstream ifile(".\\data.txt");
	//如果没有文件则，打印："打开文件失败"
	if (!ifile)
	{
		cout << "打开文件失败" << endl;
		return;
	}
	//文件存在读取出来
	while (!ifile.eof())
	{
		Info* temp = new Info;

#ifdef  _VC2010_VER_
		ifile >> *temp;
#else
		ifile >> temp->name >> temp->gender >> temp->phone >> temp->qq >> temp->email;
#endif
		//加入到链表中
		if (ifile)
		{
			AddBase(temp);
		}
	}
	//关闭流
	ifile.close();
}

void AddressBook::Save() //*将数据保存到文件*
{
	//将文件保存到
	ofstream ofile(".\\data.txt", ios::trunc);
	if (!ofile)
	{
		cout << "保存失败" << endl;
		return;
	}
	Info* temp = head;
	//写入
	while (temp != NULL)
	{

#ifdef  _VC2010_VER_
		ofile << *temp << endl;
#else
		ofile << temp->name << " " << temp->gender << " " << temp->phone << " " << temp->qq << " " << temp->email << " " << endl;
#endif
		temp = temp->next;
	}
	ofile.close();
}

void AddressBook::Add() //*添加数据*
{
	cout << "请输入联系人信息: " << endl;
	Info* temp = new Info;
	// 实际开发中这里要捕获异常:内存申请失败
	cout << "姓名( 1 ~ " << sizeof(temp->name) - 1 << " 字符): " << ends;
	cin >> temp->name;

	cout << "性别(  男   女 ): " << ends;
	cin >> temp->gender;

	cout << "电话( 1 ~ " << sizeof(temp->phone) - 1 << " 字符): " << ends;
	cin >> temp->phone;

	cout << "QQ( 1 ~ " << sizeof(temp->qq) - 1 << " 字符): " << ends;
	cin >> temp->qq;

	cout << "email( 0 ~ " << sizeof(temp->email) - 1 << " 字符): " << ends;
	cin >> temp->email;

	AddBase(temp);
}

void AddressBook::Query() //*查询联系人（按姓名或电话号）*
{
	cout << "   请选择查询方式    " << endl;
	cout << "   0. 按名字         " << endl;
	cout << "   1. 电话           " << endl;

	int choose = 0;
	cin >> choose;
	//输入查找方式
	char str[50] = { 0 };
	switch (choose)
	{
	case 0:
		cout << "请输入名字: " << ends;
		cin >> str;
		break;
	case 1:
		cout << "请输入电话: " << ends;
		cin >> str;
		break;
	default:
		cout << "没有该选项" << endl;
		return;
	}
	Info* result = head;
	//输出表头信息
	ShowInfoTitle();
	int index = 0;
	do
	{
		//查找
		result = QueryBase(result, choose, str);
		// 显示查询到的项
		if (result != NULL)
		{
			result->index = index;
			ShowInfo(result);
			index++;
			result = result->next;
		}
	} while (result != NULL);
}

void AddressBook::Modify() //*修改联系人信息*
{
	cout << "请选择要修改的联系人(输入序号): " << endl;
	//输出所有信息
	int count = ShowAllInfo();
	int index = 0;
	cin >> index;
	//判断输入的序号是否存在
	if (index < 0 || index >= count)
	{
		cout << "选择无效, 没有该序号的联系人" << endl;
		return;
	}
	//删除
	DelBase(index);
	//添加
	Add();
	//完成修改
}

void AddressBook::Del()	//*删除联系人*
{
	cout << "请选择要删除的联系人(输入序号): " << endl;
	//输出所有的信息
	int count = ShowAllInfo();
	int index = 0;
	//输入序号
	cin >> index;
	//判断序号是否存在
	if (index < 0 || index >= count)
	{
		cout << "选择无效, 没有该序号的联系人" << endl;
		return;
	}
	//存在则删除
	DelBase(index);
}

void AddressBook::ShowAll()//*输出所有信息*
{
	ShowAllInfo();
}

void AddressBook::AddBase(Info* pInfo)
{
	Info** parent = &head;
	Info* temp = head;

	while (temp)
	{
		// 按名字递增
		if (strcmp(temp->name, pInfo->name) >= 0)
		{
			break;
		}

		parent = &temp->next;
		temp = temp->next;
	}
	pInfo->next = temp;
	*parent = pInfo;
}

Info* AddressBook::QueryBase(Info* start, int choose, const char* str) //*按名字或电话号查找*
{
	while (start != NULL)
	{
		switch (choose)
		{
		case 0: // 按名字匹配
			if (strcmp(start->name, str) == 0)
			{
				return start;
			}
			else {
				start = start->next;
				continue;
			}
			break;
		case 1: // 按电话号码匹配
			if (strcmp(start->phone, str) == 0)
			{
				return start;
			}
			else {
				start = start->next;
				continue;
			}
			break;
		default:
			break;
		}
		return NULL;
	}
	return start;
}

void AddressBook::DelBase(int index) //*按序号删除*
{
	Info* temp = head;
	Info** parent = &head;
	while (temp != NULL)
	{
		if (temp->index == index)
		{
			*parent = temp->next;
			delete temp;
			return;
		}
		parent = &temp->next;
		temp = temp->next;
	}
}

void AddressBook::ShowInfoTitle() //*输出联系人信息*
{
	cout << "序号          姓名          性别          电话          QQ            email" << endl;
}

void AddressBook::ShowInfo(const Info* pInfo)
{
	cout << setiosflags(ios::left) // 输出左对齐
		<< setw(14) << pInfo->index
		<< setw(14) << pInfo->name
		<< setw(14) << pInfo->gender
		<< setw(14) << pInfo->phone
		<< setw(14) << pInfo->qq
		<< setw(14) << pInfo->email << endl;
}

int AddressBook::ShowAllInfo() //*输出序号*
{
	int index = 0;
	Info* temp = head;

	ShowInfoTitle();
	while (temp)
	{
		temp->index = index;
		index++;

		ShowInfo(temp);
		temp = temp->next;
	}
	return index;
}

void exit()//退出函数
{
	cout << "        ****************************************************************" << endl;
	cout << "        *******************          谢谢使用          *****************" << endl;
	cout << "        ****************************************************************" << endl;
}


int menu()//主菜单函数
{
	system("cls");//清屏
	cout << "        ****************************************************************" << endl;
	cout << "        |                                                              |" << endl;
	cout << "        |                    欢迎登录通讯录管理系统                    |" << endl;
	cout << "        |                                                              |" << endl;
	cout << "        ****************************************************************" << endl;
	cout << "        |                      ☆1 . 添加联系人                        |" << endl;
	cout << "        |                                                              |" << endl;
	cout << "        |                      ☆2 . 查询联系人                        |" << endl;
	cout << "        |                                                              |" << endl;
	cout << "        |                      ☆3 . 修改联系人                        |" << endl;
	cout << "        |                                                              |" << endl;
	cout << "        |                      ☆4 . 删除联系人                        |" << endl;
	cout << "        |                                                              |" << endl;
	cout << "        |                      ☆5 . 显示所有联系人                    |" << endl;
	cout << "        |                                                              |" << endl;
	cout << "        |                      ☆6 . 保存到文件                        |" << endl;
	cout << "        |                                                              |" << endl;
	cout << "        |                      ☆0 . 退出系统                          |" << endl;
	cout << "        ****************************************************************" << endl;

	int m = 0;
	do
	{
		cout << "请输入选项0-8\n";
		cin >> m;
	} while (m < 0 || m > 8);

	return m;
}