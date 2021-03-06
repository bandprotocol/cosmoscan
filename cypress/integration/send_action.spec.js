/// <reference types="cypress" />

describe("Login", () => {
  beforeEach(() => {
    cy.visit("/")
  })

  it("Should have s address on account panel", () => {
    cy.get('[id="connectButton"]').click()
    cy.get('[id="mnemonicInput"]').type("s")
    cy.get('[id="mnemonicConnectButton"] > button').click()
    cy.get('[id="userInfoButton"]').click()
    cy.get('[id="addressWrapper"]> div > a > span').should(
      "contain",
      "band1jrhuqrymzt4mnvgw8cvy3s9zhx3jj0dq30qpte"
    )
  })
})

describe("Send", () => {
  it("Status should be Success", () => {
    cy.get('[id="getFreeButton"] > button').click()
    cy.get('[id="sendToken"] > button').wait(500).contains("Send").click()
    cy.get('[id="recipientAddressInput"]')
      .wait(2000)
      .type("band1jrhuqrymzt4mnvgw8cvy3s9zhx3jj0dq30qpte")
      .get('[id="sendAmountInput"]')
      .type("2")
    cy.get('[id="nextButtonContainer"] > button').contains("Next").click()
    cy.get('[id="broadcastButtonContainer"] > button').wait(1000).click()
    cy.get('[id="successMsgContainer"] > span').should("contain", "Broadcast transaction success")
    cy.get('[id="closeModal"]').click()
  })
})

describe("Delegation", () => {
  it("Should be able to delegate with Band-Validator-9", () => {
    cy.get('[id="navigationBar"] > div > a').contains("Validator").click()
    cy.get(
      '[id="validatorsSection"] > div:nth-of-type(3) > div > div > div:nth-of-type(2) > a > span'
    )
      .wait(500)
      .contains("Band-Validator-9")
      .click()
    cy.get('[id="validatorDelegationinfoDlegate"] button:nth-of-type(1)').wait(1000).click()
    cy.get('[id="nextButtonContainer"] > button').wait(1000).should("be.disabled")
    cy.get('[id="delegateAmountInput').type("1")
    cy.get('[id="memoInput"]').type("cypress")
    cy.get('[id="nextButtonContainer"] > button').click().wait(1000)
    cy.get('[id="broadcastButtonContainer"] > button').click().wait(1000)
    cy.get('[id="successMsgContainer"] > span').should("contain", "Broadcast transaction success")
    cy.get('[id="closeModal"]').click()
  })

  it("Should be able to withdraw reward with Band-Validator-9", () => {
    cy.get('[id="withdrawRewardContainer"] > button:nth-of-type(1)').click().wait(1000)
    cy.get('[id="memoInput"]').type("cypress")
    cy.get('[id="nextButtonContainer"] > button').click().wait(1000)
    cy.get('[id="broadcastButtonContainer"] > button').click().wait(1000)
    cy.get('[id="successMsgContainer"] > span').should("contain", "Broadcast transaction success")
    cy.get('[id="closeModal"]').click()
  })

  it("Should be able to reinvest with Band-Validator-9", () => {
    cy.get('[id="withdrawRewardContainer"] > button:nth-of-type(2)').click().wait(1000)
    cy.get('[id="memoInput"]').type("cypress")
    cy.get('[id="nextButtonContainer"] > button').click().wait(1000)
    cy.get('[id="broadcastButtonContainer"] > button').click().wait(1000)
    cy.get('[id="successMsgContainer"] > span').should("contain", "Broadcast transaction success")
    cy.get('[id="closeModal"]').click()
  })
})
