<template>  
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Open+Sans:ital,wght@0,300..800;1,300..800&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css"/>

    <div>
        <h1 class="header">💊 PillCare</h1>
        
        <div class="bock">
            <div>
                <div class="text-bigger-bold">Medicine Shedule</div>
                <div class="text-medium-bold">Set your daily medication times</div>
            </div>

            <div v-for="(form, index) in forms" :key="index" class="block-input">
                <input class="input-time" type="time" v-model="form.time" step="60"/>
                <div class="input-slot">{{ form.slot }}</div>
                <input class="input-list" type="text" v-model="form.list" placeholder="Medicine List" class="input-list"/>
                <button @click="removeForm(index)" class="button-remove">
                    <i class="fa-regular fa-trash-can"></i>
                </button>

            </div>

            <button v-if="forms.length < 8" class="button-block-add" @click="addForm">
                <div class="button-font-add">+  Add Medicine Time</div>
            </button>
        </div>
        <button class="button-block" @click="saveAll"> 
            <div class="button-font">Save All Setting</div>
        </button>
    </div>
</template>

<script>
export default {
    data() {
        return {
            forms: []
        };
    },
    methods: {
        addForm() {
            if (this.forms.length < 8) {
                this.forms.push({
                    time: "",
                    slot: this.forms.length + 1,
                    list: ""
                });
            }
        },
        removeForm(index) {
            this.forms.splice(index, 1);
            this.forms.forEach((f, i) => f.slot = i + 1);
        },
        saveAll() {
            const payload = this.forms.map(f => ({
                time: f.time,
                slot: f.slot,
                list: f.list
            }));

            this.send({ payload }); 
        },
        loadJson(payload) {
            this.forms = payload.map((item, index) =>({
                 time: item.time,
                slot: item.slot,
                list: item.list
            }));
        }
    }
};
</script>


<style scoped>

<!-- .font-test {
    font-size: 80%;
} -->
.bock {
    border: 1px solid rgb(201, 201, 201);
    border-radius: 10px;
    padding: 30px;
    margin: 20px auto;
    max-width: 70%;
    flex-wrap: wrap;
    justify-content: space-between;
    align-items: center;
    box-sizing: border-box;
    min-width:525px
}

@media (max-width: 600px) {
    .bock {
        margin-left: 10px;
        margin-right: 10px;
        max-width: calc(100% - 20px);
        min-width: 0px;
    }
}

.button-block {
    width: 100%;                
    max-width: 70%;             
    margin: 20px auto;          
    padding: 10px 20px;          
    border-radius: 10px;
    background-color: #23a755;
    display: flex;
    justify-content: center;      
    box-sizing: border-box;     
    min-width:525px  
}

.button-block:hover {
    background-color: #1e8e4a; 
    transition: all 0.2s ease-in-out; 
}

@media (max-width: 600px) {
    .button-block {
        margin-left: 10px;
        margin-right: 10px;
        max-width: calc(100% - 20px);
        min-width: 0px;
    }
}

.button-block-add {
    margin-top: 20px;
    padding: 10px 20px;       
    width: 100%;            
    max-width: 100%;            
    margin-left: auto;
    margin-right: auto;
    border: 1px solid rgb(201, 201, 201);
    border-radius: 10px;
    background: transparent;
    text-align: center;        
    box-sizing: border-box;
}

.button-block-add:hover {
    background-color: #f0f0f0;
    transition: all 0.2s ease-in-out; 
}

.block-input {
    margin-top: 20px;
    padding: 10px;
    max-width: 100%;
    margin-left: auto;
    margin-right: auto;
    border: 1px solid rgb(201, 201, 201);
    border-radius: 10px;
    background: transparent;
    text-align: center;        
    box-sizing: border-box;
}

.add-block {
    border-radius: 10px;
    background-color: #f0f0f0;
}

.header {
    font-family: "Open Sans", sans-serif;
    font-size: 1.75rem;    
    line-height: 2.25rem;  
    font-weight: 700;      
    color: #2daa28;
}

.text-bigger-bold {
    font-family: "Open Sans", sans-serif;
    font-optical-sizing: auto;
    font-style: normal;
    font-variation-settings: "wdth" 100;
    font-size: 1.5rem;    
    line-height: 2rem;    
    font-weight: 525;  
    color: #333333;
}

.text-medium-bold {
    font-family: "Open Sans", sans-serif;
    font-optical-sizing: auto;
    font-style: normal;
    font-variation-settings: "wdth" 100;
    font-size: 1.125rem;       
    line-height: 1.625rem;    
    font-weight: 400;      
    color: #505050;         
}

.button-font {
    font-family: "Open Sans", sans-serif;
    font-size: 1rem;    
    line-height: 1.5rem;    
    font-weight: bold;  
    color: #ffffff;  
    text-align: center;
}

.button-font-add {
    font-family: "Open Sans", sans-serif;
    font-size: 1rem;    
    line-height: 1.5rem;    
    font-weight: bold;  
    color: #333333;  
    text-align: center;
}

@media (max-width: 600px) {
    .header { font-size: 1.4rem; }
    .text-bigger-bold { font-size: 1.2rem; }
    .text-medium-bold { font-size: 1rem; }

    .bock {
        max-width: calc(100% - 20px);
        padding: 1rem;
        margin: 10px;
    }
}

.block-input {
    display: flex;
    align-items: center;
    gap: 10px;
    margin-top: 20px;
    padding: 10px;
    border: 1px solid rgb(201, 201, 201);
    border-radius: 10px;
    box-sizing: border-box;
    font-family: "Open Sans", sans-serif;
    font-size: 1rem;    
    line-height: 1.5rem;    
    font-weight: 500;  
    color: #333333;  
    text-align: center;
}

.input-time {
    flex: 1;               
    min-width: 130px;      
    max-width: 180px; 
    border-radius: 10px;
    text-align: center;
    padding: 10px;
    background-color: #f0f0f0;
    text-align: center;
}

.input-slot {
    width: 50px;           
    flex: 0 0 50px;
    border-radius: 10px;
    padding: 10px;
    background-color: #f0f0f0;
    text-align: center;
}

.input-list {
    flex: 2;
    min-width: 150px;
    border-radius: 10px;
    padding: 10px;
    background-color: #f0f0f0;
    text-align: center;
    color: #333333; 
}

.button-remove {
    width: 50px;           
    flex: 0 0 50px;
    border-radius: 10px;
    padding: 10px;
    background-color: #ffffff;
    text-align: center;
    border: 1px solid rgb(201, 201, 201);
    transition: background-color 0.2s ease-in-out;
}

.button-remove i {
    color: #505050;
    font-size: 18px;
    transition: color 0.2s ease-in-out; 
}

.button-remove:hover {
    background-color: #d64d4d;
}

.button-remove:hover i {
    color: #ffffff;
}

.input-list:focus-within,
.input-slot:focus-within,
.input-time:focus-within {
    border-color: #333333;
    box-shadow: 0 0 5px 1px rgba(0,0,0,0.3);
    transition: box-shadow 0.2s ease-in-out, border-color 0.2s ease-in-out;
    outline: none;
}

@media (max-width: 600px) {
    .block-input {
        display: grid;
        grid-template-columns: 1fr auto; 
        grid-template-rows: auto auto;   
        gap: 10px;
        align-items: center;
     }

    .input-time { 
        grid-column: 1; 
        grid-row: 1;
        max-width: none;
        min-width: none;
    }
    .input-slot { 
        grid-column: 2; 
        grid-row: 1; 
    }
    .input-list { 
        grid-column: 1;
        grid-row: 2; 
    }
    .button-remove { 
        grid-column: 2; 
        grid-row: 2; 
    }
}
</style>