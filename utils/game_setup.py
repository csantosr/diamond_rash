from selenium.common.exceptions import NoSuchElementException
from selenium.webdriver.common.by import By


class GameSetup:

    @staticmethod
    def accept_privacy(driver):
        try:
            button = driver.find_element(
                By.XPATH, '//*[@id="qc-cmp2-ui"]/div[3]/div/button[2]'
            )
            button.click()
        except NoSuchElementException:
            pass

    @staticmethod
    def click_play(driver):
        try:
            button = driver.find_element(
                By.XPATH, '//*[@id="clickToPlayButton"]'
            )
            button.click()
        except NoSuchElementException:
            pass
