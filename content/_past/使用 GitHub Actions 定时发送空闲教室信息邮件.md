---
title: 使用 GitHub Actions 定时发送空闲教室信息邮件
description: 用 GitHub Actions 写一个定时任务，每天早上把当天的空闲教室信息写进邮件发给我
toc: true
categories: 开源项目
tags:
- python
- 爬虫
- 验证码 
date: 2022-09-20T19:23:09+08:00
lastmod: 2022-09-20T19:23:09+08:00
image: https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/mmexport1624495514000.png
draft: false
---

书接上回 -> [URP教务系统自动登录](https://blog.wangjialei.xyz/posts/urp%E6%95%99%E5%8A%A1%E7%B3%BB%E7%BB%9F%E8%87%AA%E5%8A%A8%E7%99%BB%E5%BD%95/)

> 基本想法完成了，但是，比较困扰我的一点是，查询到的数据该以什么样的方式存储起来，这一点可能还需要我好好想想，或许会写个接口来传输数据，然后写个App方便查询？还不太确定。

昨天刷知乎，突然灵光一现：我可以用 GitHub Actions 写一个定时任务，每天早上把当天的空闲教室信息写进邮件发给我。说干就干。

## 查询当天空闲教室

### 获取当天日期

这个说起来简单，但是还是坑了我，GitHub Actions 使用的 [UTC](https://www.timeanddate.com/worldclock/timezone/utc) 标准时间，我们这边的时区是 UTC+8，快8个小时，所以如果是早上8点之前执行的话，获取的还是昨天的信息。这里的时间要改。

定义三个函数，方便后续使用

```python
# only for 2022-2023 1
def get_week_num(d=datetime.now(pytz.timezone('Asia/Shanghai'))):
    return d.isocalendar().week - 34


# mon -> 1, ect..
def get_weekday(d=datetime.now(pytz.timezone('Asia/Shanghai'))):
    return datetime.weekday(d) + 1


def get_format_date(d=datetime.now(pytz.timezone('Asia/Shanghai'))):
    return d.strftime('%Y年%m月%d日')
```

这里我们使用 `pytz` 库来获取 `Asia/Shanghai` 时区的当前时间。

`get_week_num()` 函数用于获取当前的周数，这里的周数是学校的教学周，正好这学期2023年就没有教学周了，所以简单粗暴的相减即可

`get_weekday()` 函数获取当前是一周的第几天

### 根据日期进行查询

定义每一小节的时间段

```python
class_time = {
    1: '第1节 08:00 08:45',
    2: '第2节 08:50 09:35',
    3: '第3节 09:50 10:35',
    4: '第4节 10:40 11:25',
    5: '第5节 11:30 12:15',
    6: '第6节 14:00 14:45',
    7: '第7节 14:50 15:35',
    8: '第8节 15:50 16:35',
    9: '第9节 16:40 17:25',
    10: '第10节 18:30 19:15',
    11: '第11节 19:20 20:05',
    12: '第12节 20:10 20:55',
}
```

因为是每天的空闲教室信息，这里需要批量查询每一小节的教室使用情况。
其他校区的同学可自行添加其他校区的信息。

```python
def search_today(self):
    campus_code = 3             # 校区代码
    tea_codes = [61, 62]        # 教学楼代码列表
    week_num = util.get_weekday()    # 周数
    results = []                # 结果集
    for tea_code in tea_codes:
        classrooms = []
        for i in range(1, 13):
            section = str(week_num) + '/' + str(i)  # 节数
            param = {
                'weeks': util.get_week_num(),
                'jslxdm': '',
                'codeCampusListNumber': campus_code,
                'teaNum': tea_code,
                'wSection': section,
                'pageNum': 1,
                'pageSize': 100,
            }
            ret = self.search_free_classroom(param)
            classroom = {
                'time': class_time.get(i),
                'rooms': ", ".join(ret),
            }
            classrooms.append(classroom)
        results.append({
            'tea': teaching_num.get(tea_code),
            'classrooms': classrooms,
        })
    return results
```

## 创建邮件模板

这里是个大坑，一开始我不知道邮件HTML要求特别严格，有的邮件客户端甚至会过滤掉 `<head>` 里面的 `<style>` 外部样式

更多的内容可以参考阮一峰的博客 [HTML Email 编写指南](https://www.ruanyifeng.com/blog/2013/06/html_email.html)

我使用了 [JinJa2](http://docs.jinkan.org/docs/jinja2/) 作为HMTL文件的模板渲染语言。

这里贴出模板的主要内容。

```html
<h3 style="text-align: center;">空闲教室</h3>
<p style="text-align: center; padding-bottom: 20px; color: #586069">今日（{{ daytime }}）</p>
{% for result in results %}
<p style="margin-left: 20px">{{ result.tea }}空闲教室</p>
<table class="table table-striped table-bordered" border="1" style="border-collapse: collapse; margin-bottom: 30px">
  <thead>
    <tr>
      <th class="content-block" style="width: 130px">节次</th>
      <th>空闲教室</th>
    </tr>
  </thead>
  <tbody>
    {% for classroom in result.classrooms %}
    <tr>
      <td class="content-block" style="width: 130px"><p>{{ classroom.time }}</p></td>
      <td style="color: #586069">{{ classroom.rooms }}</td>
    </tr>
    {% endfor %}
  </tbody>
</table>
{% endfor %}
```

渲染的内容包括两个方面：

- `daytime`: 当天时间的格式化字符串
- `results`: 查询结果的Map对象

渲染模板部分的代码：

```python
TEMPLATE_FILE = glob.glob('template/mail.j2')[0]
HTML_FILE = 'template/result.html'


# write string to html using template
# params: map
# template_file: jinja2 template
# filename: html file
def write2html(params, template_file=TEMPLATE_FILE, html_file=HTML_FILE):
    current_directory = os.path.dirname(os.path.abspath(__file__))
    env = Environment(loader=FileSystemLoader(current_directory))
    res = env.get_template(template_file).render(params)
    test = codecs.open(html_file, 'w', 'utf-8')
    test.write(res)
    test.close()
```

函数 `write2html()` 接受待渲染的内容 `params`，读取模板文件 `template_file`，将内容输出到文件 `html_file` 中。

## 重构代码

为了方便测试和后续部署，这里我们的URP教务系统的用户名和密码不再硬编码到文件中，而是使用命令行参数的形式进行读取。

```python
# read parameters from arguments
# -u or --username: username
# -p or --password: password
def parse_params():
    parser = argparse.ArgumentParser()
    parser.add_argument("-u", "--username", dest="username")
    parser.add_argument("-p", "--password", dest="password")
    args = parser.parse_args()
    return args.__getattribute__('username'), args.__getattribute__('password')
```

运行时只需要使用下列命令即可
```
python main.py -u <username> -p <password>
```

## 添加定时任务

> GitHub Actions makes it easy to automate all your software workflows, now with world-class CI/CD. Build, test, and deploy your code right from GitHub. Make code reviews, branch management, and issue triaging work the way you want.

我们可以在 GitHub Actions 上创建自己的 workflow ，命名为 `send mail`

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-09-20_20-03-01.png)

更多关于 GitHub Actions 的内容，可以参考官方文档 [https://docs.github.com/cn/actions](https://docs.github.com/cn/actions)

这里简单介绍一下构建流程:

1. 拉取 ubuntu 镜像

2. 切换到 actions 分支

3. 创建 python 工作环境

4. 安装依赖

5. 执行 python 脚本

6. 发送邮件

```yaml
name: send mail

on:
#  push:
  schedule:
    - cron: "20 22 * * *"

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - name: checkout repo content
      uses: actions/checkout@v2
    - name: setup python
      uses: actions/setup-python@v2
      with:
        python-version: 3.9
    - name: install dependencies
      run: |
        pip install -r requirement.txt
    - name: execute py script
      run: |
        python main.py -u $Username -p $Password
      env:
        Username: ${{ secrets.HHU_USERNAME }}
        Password: ${{ secrets.HHU_PASSWORD }}
    - name: send email
      uses: dawidd6/action-send-mail@v2
      with:
        server_address: smtp.qcloudmail.com
        server_port: 465
        username: ${{ secrets.MAIL_USERNAME }}
        password: ${{ secrets.MAIL_PASSWORD }}
        subject: 今日空闲教室
        body: file://template/result.html
        to: ${{ secrets.EMAIL }}
        from: GitHub Actions
        content_type: text/html
```

为了防止密码泄露，我们还要在仓库添加5个密钥：

- `HHU_USERNAME`: URP 教务系统用户名

- `HHU_PASSWORD`: URP 教务系统密码

- `MAIL_USERNAME`: 邮箱服务发件邮箱

- `MAIL_PASSWORD`: 邮箱服务发件邮箱授权密码

- `EMAIL`: 收件邮箱

另外，根据不同的邮件服务，`server_address` 和 `server_port`一般也不同，需要自行配置。

我们将运行时间定为北京时间每天早上6:20，为了方便查看效果，测试时可以另外设置为`push`时触发。

差不多就是这样，每天早上起床就可以收到邮件，不需要一天登录教务多次，反复查看，完美。

以上。
