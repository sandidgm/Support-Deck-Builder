using namespace std;

#include "declarations.h"
#include "classes.h"

/**
    This is the main function of my support deck solver. From here you may enter into three different functions:
        1) Brute force support deck combination solver
        2) Statistics analysis
        3) Print to screen info about cards
**/


int main(){

    int x,i, z=0, choice, get_skill, new_search, translator, keep_adding;
    int effective_chars=0, using_rarity=4, path, not_done=1, is_double=0;
    ifstream myfile;

    srand(time(NULL));

    //the following four functions create the data used by the program
    create_Database(&allsupportskills, &all_cards, &unique_affiliations, &affiliation_array);

    do{
        cout << "Check combinations (1) or check stats (2) or print profile info (3) or Quit (0)? :  ";
        cin >> path;

        while(!(cin.good()) || path<0 || path>3){
            cin.clear();
            cin.ignore(256, '\n');
            cout << "Please enter a valid choice: ";
            cin >> path;
        }

        if(path==1){
            reset_support_deck(&supportdeck);

            cout << "\n\nHow many skills (2 - 6) do you want in your support deck?: ";
            cin >> supportdeck.number_of_skills;

            while(!(cin.good()) || supportdeck.number_of_skills>6 || supportdeck.number_of_skills<2){
                cin.clear();
                cin.ignore(256, '\n');
                cout << "Please enter a number from 2 to 6: ";
                cin >>supportdeck.number_of_skills;
            }

            if(supportdeck.number_of_skills>1){
                cout << "\nEnforce minimum skill levels? (Yes = 1): ";
                cin >> choice;

                supportdeck.looking_for_specific_level=1;

                if(!(cin.good()) || choice!=1){
                    cin.clear();
                    cin.ignore(256, '\n');
                    choice=0;
                    supportdeck.looking_for_specific_level=0;
                }


                cout << endl;
                for(z=0;z<allsupportskills.numberOfSkills;z++){
                    cout << z+1 << ") " << allsupportskills.supportskill[z].skillName << endl;
                }
                cout << endl << endl;

                for(x=0;x<supportdeck.number_of_skills;x++){
                    do{
                        cout << "Please choose a skill: ";
                        cin >> get_skill;

                        while(!(cin.good()) ){
                            if( !(cin >> get_skill) ){
                                cin.clear();
                                cin.ignore(256, '\n');
                            }
                        }

                        if(get_skill>allsupportskills.numberOfSkills || get_skill<1){
                            cout << "\n\tPlease enter a number from 1 to " << allsupportskills.numberOfSkills << endl << endl;
                        }

                        is_double=0;

                        for(i=0;i<x;i++){
                            if(get_skill-1==supportdeck.skill_locator[i]){
                                is_double=1;
                            }
                        }

                        if(is_double==1){
                            cout << "\n\tSkill already chosen - choose a different one.\n\n";
                        }



                    }while(get_skill>allsupportskills.numberOfSkills || get_skill<1 || is_double==1);

                    supportdeck.skills_in_deck[x]=allsupportskills.supportskill[get_skill-1].skillName;
                    supportdeck.skill_locator[x]=get_skill-1;

                    for(i=0;i<allsupportskills.supportskill[get_skill-1].numberOfCards;i++){
                        cout << i+1 << ") " << allsupportskills.supportskill[get_skill-1].supportskillcard[i].rarity << "* " << allsupportskills.supportskill[get_skill-1].supportskillcard[i].charactername << "\n";
                    }

                    cout << "\nIgnore any skill cards? Enter the number of those you don't want to search - enter 0 when ready to proceed:  ";

                    keep_adding=0;

                    do{
                        cin>>translator;

                        if(!(cin.good()) || translator<0 || translator>allsupportskills.supportskill[get_skill-1].numberOfCards){
                            cin.clear();
                            cin.ignore(256,'\n');
                            cout << "\n\tInvalid entry\n";
                        }else
                            if(translator==0){
                                break;
                            }else{
                                new_search=0;

                                for(i=0;i<keep_adding;i++){
                                    if(supportdeck.ignore_list[x][i]!=(translator-1)){
                                        new_search++;
                                    }
                                }

                                if(new_search==keep_adding){
                                    supportdeck.ignore_list[x][keep_adding]=translator-1;
                                    keep_adding++;
                                }
                            }

                        if(keep_adding == (allsupportskills.supportskill[get_skill-1].numberOfCards-1) ){
                            break;
                        }

                    }while(1==1);

                    supportdeck.num_ignored_cards[x]=keep_adding;
                    supportdeck.num_allowed_cards[x]=allsupportskills.supportskill[get_skill-1].numberOfCards-keep_adding;

                    if(choice==1){
                        if(allsupportskills.supportskill[get_skill-1].max_level==1){
                            supportdeck.skill_threshold[x]=1;
                        }else{
                            cout << "\nSet minimum skill level (";
                            for(z=0;z<allsupportskills.supportskill[get_skill-1].max_level;z++){
                                cout << z+1;

                                if(z+1<allsupportskills.supportskill[get_skill-1].max_level){
                                    cout << ", ";
                                }else{
                                    cout << "): ";
                                }
                            }

                            cin >> supportdeck.skill_threshold[x];

                            while(!(cin.good()) || supportdeck.skill_threshold[x]>allsupportskills.supportskill[get_skill-1].max_level || supportdeck.skill_threshold[x]<1){
                                cout << "\nInvalid value. Enter again: ";
                                cin.clear();
                                cin.ignore(256, '\n');
                                cin >> supportdeck.skill_threshold[x];
                            }
                        }

                    }else{
                        supportdeck.skill_threshold[x]=1;
                    }
                    cout << "\n\n";
                }

                cout << "Enter the minimum number of matching types per card: ";
                cin >> supportdeck.type_threshold;

                while(!(cin.good()) || supportdeck.type_threshold<0 || supportdeck.type_threshold>MAXTYPES){
                    cout << "\nInvalid value. Enter again: ";
                    cin.clear();
                    cin.ignore(256, '\n');
                    cin >> supportdeck.type_threshold;

                }

                cout << "\nQuery through which rarity (1 2 3 4 5)? : ";
                cin >> using_rarity;

                if( !(cin.good()) || using_rarity<1 || using_rarity>5){
                    cin.clear();
                    cin.ignore(256, '\n');
                    using_rarity=4;
                }

                effective_chars=0;

                for(i=0;i<unique_affiliations.number_of_characters;i++){
                    if(unique_affiliations.card[i].rarity>=using_rarity){
                        effective_chars++;
                    }
                }

                cout << "Stop searching after a certain number of solutions? (Yes = 1):  ";
                cin >> supportdeck.breaking_early;

                if( !(cin.good()) || supportdeck.breaking_early!=1){
                    cin.clear();
                    cin.ignore(256, '\n');
                    supportdeck.breaking_early=0;
                    supportdeck.breaking_at_solution=0;
                }else{
                    cout << "Search for how many solutions?:  ";
                    cin >> supportdeck.breaking_at_solution;

                    while(!(cin.good()) || supportdeck.breaking_at_solution<1){
                        cout << "\nInvalid value. Enter again: ";
                        cin.clear();
                        cin.ignore(256, '\n');
                        cin >> supportdeck.breaking_at_solution;

                    }
                }

                construct_support_deck(&supportdeck, &allsupportskills, &all_cards, &unique_affiliations, effective_chars);

            }else{
                cout << "Thanks for using the support skill optimizer!!";
            }

        }else
            if(path==2){
                check_stats(&all_cards,&unique_affiliations,&affiliation_array, &allsupportskills);
            }else
                if(path==3){
                    print_info(&allsupportskills,&all_cards);
                }else
                    if(path==0){
                        not_done=0;
                    }

        if(path!=0){
            cout << "\n\nDo something else? (Yes = 1): ";
            cin >> not_done;

            if( !(cin.good()) || not_done!=1){
                cin.clear();
                cin.ignore(256, '\n');
                not_done=0;
            }
        }

    }while(not_done==1);

    return 0;
}
