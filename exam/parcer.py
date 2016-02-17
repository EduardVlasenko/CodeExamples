import requests
import bs4
import re


class Employee:
    def __init__(self, page):
        self._homepage_url = page
        page = requests.get(page)
        self._homepage = bs4.BeautifulSoup(page.text, "html.parser")  # Should I save parsed page somewhere?

        self._full_name = self.homepage.find_all("div", attrs={"class": "main__inner main__inner_"})[0].find("h1").text

        self._affiliations = None

        contacts = self.homepage.find("dl", attrs={"class": "main-list large"})
        emails = contacts.find(text="Электронная почта:")
        if emails is not None:
            emails = str(emails.parent.script.text)
            emails = eval(("".join(emails.split())).replace("hseEObfuscator", "").replace(";", "").
                          replace(")", "]").replace("(", "["))
            self._emails = ["".join(email) for email in emails]
        else:
            self._emails = []

        try:
            phone_numbers = contacts.find(text="Телефон:").parent
            self._phone_numbers = str(phone_numbers.br).replace("</br>", "").split("<br>")[1:]
        except AttributeError:
            self._phone_numbers = []

        self._keywords = []
        person_data_tags = self.homepage.find_all("div", attrs={"class": "b-person-data b-person-data__tags",
                                                                   "tab-node": "sci-intrests"})
        if person_data_tags:
            person_data_tags = person_data_tags[0].find_all("a")
            for a in person_data_tags:
                try:
                    keyword_number = int(a["href"].replace("/org/persons/?intst=", ''))
                except ValueError:
                    keyword_number = -1
                self._keywords.append((keyword_number, a.text))

    @property
    def homepage(self):
        return self._homepage

    @property
    def full_name(self):
        return self._full_name

    @property
    def affiliations(self):
        if self._affiliations is None:
            pass
        return self._affiliations

    @property
    def emails(self):
        return self._emails

    @property
    def phone_numbers(self):
        return self._phone_numbers

    @property
    def keywords(self):
        return self._keywords


class Department:
    parsed_departments = dict()

    def __new__(cls, udept):
        if udept in cls.parsed_departments:
            return cls.parsed_departments[udept]
        else:
            return object.__new__(cls)

    def __init__(self, udept):
        if udept in type(self).parsed_departments:
            return
        self._page_url = "http://www.hse.ru/org/persons/?udept=" + str(udept)
        page = requests.get(self._page_url)
        self._staff_page = bs4.BeautifulSoup(page.text, "html.parser")

        title_current_part = self.staff_page.find("span", attrs={"class": "pseudo js-pseudo selected b"})
        self._title = title_current_part.find("a").text
        title_last_part = self._title
        title_current_part = title_current_part.parent
        while self._title[0].islower():
            title_current_part = title_current_part.parent.parent    # добавить определение родителей?
            self._title = title_current_part.find("a").text + ' / ' + self._title

        persons = self.staff_page.find_all("div", attrs={"class": "post person"})
        not_letters = re.compile("[\W]").finditer(title_last_part)
        for not_letter in not_letters:
            title_last_part = title_last_part[:not_letter.start()] + '.' + title_last_part[not_letter.end():]
        self._staff_urls = []
        for person in persons:
            title_last_part
            while True:
                try:
                    text = str(person.p)
                    (i, j) = re.compile(title_last_part, re.IGNORECASE).search(text).span()
                    i = text.rfind(': ', 0, i)
                    position = text[text.rfind('>', 0, i) + 1: i]
                    if text[j: j+6].find(' /') >= 0:  # or set position empty
                        k = text.find('</p>', j)
                        if text.find('<br>', j) > 0:
                            k = min(text.find('<br>', j), k)
                        position_tail = text[j: k]
                        tags = list(re.compile('<[^>]*>').finditer(position_tail))
                        for tag_location in reversed(tags):
                            position_tail = position_tail[:tag_location.start()] + position_tail[tag_location.end():]
                        position += ':' + position_tail
                    break
                except AttributeError:
                    position = ''
                    break
                except StopIteration:
                    if title_last_part.find('.') < 0:
                        position = ''
                        break
                    else:
                        title_last_part = title_last_part[:title_last_part.rfind('.')]
            self._staff_urls.append((
                'http://www.hse.ru' + person.find("a", attrs={'class': 'link link_dark large b'})['href'],
                position))

        self._staff = []
        type(self).parsed_departments[udept] = self

    @property
    def staff_page(self):
        return self._staff_page

    @property
    def title(self):
        return self._title

    @property
    def staff(self):  # Parsing of all personal may took a long time.
        if not self._staff:
            self._staff = [(Employee(url), position) for url, position in self._staff_urls]
        return self._staff

    def refresh(self):
        udept = int(self._page_url.split("=")[1])
        del type(self).parsed_departments[udept]
        self.__init__(udept)
        return


dep = Department(122804282)
print(dep.staff[1][0].full_name)
print(dep.staff[1][0].keywords)

