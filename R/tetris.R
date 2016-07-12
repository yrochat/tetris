#!/usr/bin/Rscript
# ENABLE command line arguments

setwd("~/Dropbox/Projects/Twitterbots/tetris/R")

rm(list=ls())

library(httr)
library(twitteR)
library(stringr)
library(dplyr)

# callback url http://127.0.0.1:1410

source("my_tokens.R")

options(httr_oauth_cache=TRUE) 
setup_twitter_oauth(api_key, api_secret, access_token, access_token_secret)




# DEPUIS LE DERNIER TWEET

ment <- mentions()
ment.df <- twListToDF(ment)

# FAIRE UN GREP SUR LES COMMANDES SUIVANTES

directions <- matrix(c("droite", "d",
				"right", "d",
				"➡", "d",
                "gauche", "l",
                "left", "l",
                "\u2b05", "l",
                "bas", "d",
                "down", "d",
                "\u2b07", "d",
                "spin", "s",
                "antispin", "a",
                "fall", "f"), ncol = 2, byrow = T)
                
directions <- as.data.frame(directions, stringsAsFactors = F)
colnames(directions) <- c("input", "output")

str_match(ment.df$text, directions$input)



# S'IL NE RECUPERE RIEN IL FAIT DOWN


# LA COMMANDE À LANCER 
# Générer le prochain état en lancant le code c++ : $ TwitterTetris --last last_state.xml --img img03.png --action f


# POUR PLUS TARD 
# Générer une animation avec les derniers états. On pourrait numéroter les images de 1 à infty et stocker l'info du nombre d'actions précédemment effectuées dans le fichier xml d'état pour faciliter la chose.
 
# POUR PLUS TARD 
# Si des lignes sont effacées, TwitterTetris génère des images supplémentaires avec l'animation ou directement un gif à chaque fois plut^t que le png ?
 
 
# Tweeter l'animation avec l'info du joueur ayant effectué la dernière action ou game over.