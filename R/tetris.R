#!/usr/bin/Rscript
# ENABLE command line arguments

setwd("~/Dropbox/Projects/Twitterbots/tetris/R")

rm(list=ls())

library(httr)
library(twitteR)
library(stringr)
library(dplyr)


# callback url http://127.0.0.1:1410
# to enter at https://apps.twitter.com/
# this is standard for twitter + R


# WE GREP ALL POTENTIAL INPUT COMMANDS

directions <- matrix(c("droite", "r",
                       "right", "r",
                       "➡", "r",
                       "gauche", "l",
                       "left", "l",
                       "\u2b05", "l",
                       "bas", "d",
                       "down", "d",
                       "\u2b07", "d",
                       "spin", "s",
                       "antispin", "a",
                       "fall", "f"), 
                       ncol = 2, 
                       byrow = T)
                
directions <- as.data.frame(directions, stringsAsFactors = F)
colnames(directions) <- c("input", "output")

directions.all <- str_c(directions$input, collapse = "|")


# LOAD TWITTER TOKENS. THIS IS NOT SHARED PUBLICLY

source("my_tokens.R")


# twitteR AUTHENTICATION

options(httr_oauth_cache=TRUE) 
setup_twitter_oauth(api_key, api_secret, access_token, access_token_secret)


# SINCE THE LAST TWEET

ment <- mentions()
ment.df <- twListToDF(ment)


# SAVE MAX ID FOR LATER

id.max <- as.character(max(ment.df$id))


# PICK A COMMAND

id.commands.TF <- str_detect(ment.df$text, directions.all)


# IF NO AVAILABLE COMMAND, IT'S GONNA BE DOWN ("d")

if (sum(id.commands.TF) == 0) {
    input <- "d"
    
# IF ONLY ONE COMMAND

} else if (sum(id.commands.TF) == 1) {
    input.df <- ment.df
    input <- directions$output[directions$input == str_extract(input.df$text, directions.all)[1]]

# IF MORE THAN ONE

} else {
    input.df <- ment.df[sample(which(id.commands.TF), 1),]
    input <- directions$output[directions$input == str_extract(input.df$text, directions.all)[1]]
}


# LA COMMANDE À LANCER 
# Générer le prochain état en lancant le code c++ : $ TwitterTetris --last last_state.xml --img img03.png --action f


# POUR PLUS TARD 
# Générer une animation avec les derniers états. On pourrait numéroter les images de 1 à infty et stocker l'info du nombre d'actions précédemment effectuées dans le fichier xml d'état pour faciliter la chose.
 
# POUR PLUS TARD 
# Si des lignes sont effacées, TwitterTetris génère des images supplémentaires avec l'animation ou directement un gif à chaque fois plut^t que le png ?
 
 
# Tweeter l'animation avec l'info du joueur ayant effectué la dernière action ou game over.